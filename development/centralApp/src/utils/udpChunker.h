/*
 *  udpChunker.h
 *  openFrameworks
 *
 *  Created by Zachary Lieberman on 10/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

#pragma once



//----------------------------------------------------------------
class dataChunk{
	
	public: 
	
	unsigned char *			data;
	int length;							// used for sending, other stuff

	dataChunk(){
		length = 0;
		data = NULL;
	}
	
	~dataChunk(){
		if(data != NULL){
			//printf("deleteing data \n");
			delete [] data;
		}
	}
	
};



class chunkedPacket  {
	
	public:
	
	chunkedPacket(){
		bReceived = NULL;
		chunks = NULL;
		packetID = -1;
	}
	
	~chunkedPacket(){
		
		
		
		if(chunks != NULL){
			//printf("deleteing chunks \n");
			delete [] chunks;
		}
		
		if(bReceived != NULL){
			//printf("deleteing received \n");
			delete [] bReceived;
		}
	}
	
	void setup(int _nChunks, int chunkSize){
		
		nChunks		= _nChunks;
		chunks		= new dataChunk[nChunks];
		bReceived	= new bool[nChunks];
		
		for (int i = 0; i < nChunks; i++){
			
			chunks[i].data = new unsigned char[chunkSize];
			memset(chunks[i].data, 0, chunkSize);
			bReceived[i] = false;
		}
		
	}
	
	bool bAmIFull(){
		bool bFull = true;
		for (int i = 0; i < nChunks; i++){
			if (!bReceived[i]){
				bFull = false;
			}
		}
		return bFull;
	}
	
	void addChunk(int nPosition, unsigned char * data, int length){
		bReceived[nPosition] = true;
		memcpy(chunks[nPosition].data, data, length);
		chunks[nPosition].length = length;
	}
	

	bool		* bReceived;
	dataChunk	* chunks;
	int			nChunks;
	
	long long	packetID;
};
//----------------------------------------------------------------






class udpChunker {

public: 

	
	
	//-----------------------------------------------------------------------------
	void setup( int _nBytesOfNormalUDP,   int _goodChunkSize){
		
		
		latestDataPacket	= new unsigned char [_nBytesOfNormalUDP];
		nBytesOfNormalUDP	= _nBytesOfNormalUDP;
		goodChunkSize		= _goodChunkSize;
		nChunks				= ceil( (float)nBytesOfNormalUDP / (float)goodChunkSize);   // the last chunk might not be resonable..  fine. 
		
		cout << "creating chunks that are from data " << _nBytesOfNormalUDP << " " << goodChunkSize << " " << nChunks << " " << endl;
		// do something!! 
		
		lastGoodPacketTime = ofGetElapsedTimef();
		
		chunk = new unsigned char [_goodChunkSize];
		
		CP.setup(nChunks,  goodChunkSize);
		
		newestData = -1;
	}
	
	
	//-----------------------------------------------------------------------------
	// SENDING STUFF 
	//-----------------------------------------------------------------------------
	void convertToChunkedPacketForSending(unsigned char * data, int length ){
		
		/*if (length != goodChunkSize){
			cout << "some error in convert chunked size.. check setup vs. usage " << endl;
			return;
		}
		*/
		
		// let's do this!! 
		
		int totalLength = 0;
		for (int i = 0; i < nChunks; i++){
			int startPos = i * goodChunkSize; 
			int endPos = MIN((i+1) * goodChunkSize, length); 
			int thisLength = endPos - startPos;
			unsigned char * ptr = data + startPos;
			
			totalLength += thisLength;
			//cout << "adding chunk " << i << " length = " << thisLength << endl;
		}
		//cout << " my totalLength " << totalLength << " " << length << endl;

	}
	
	dataChunk * getChunk (int id) {
		return CP.chunks + id;
		
	}

	int				nChunks;
	int				nBytesOfNormalUDP;
	int				goodChunkSize;
	chunkedPacket	CP;
	unsigned char *	chunk;
	
	
	
	
	//-----------------------------------------------------------------------------
	// RECEIVING STUFF 
	//-----------------------------------------------------------------------------
	
	vector <chunkedPacket * >	packets;
	unsigned char *				latestDataPacket;
	int							newestData;	// new = bigger!
	float						lastGoodPacketTime;
	
	void receiveDataChunk( unsigned char * data, int whichPosition, long long whichPacket){
		
		// this ** shuold be reset ** after a certain amount of time of none finding a good packet 
		
		if (whichPacket <= newestData) {
			printf("skipping this data from packet %i, since we've received a packet already \n", whichPacket); 
			return; // drop this data
		}
		
		
		bool bDoesThisPacketExistAnywhere = false;
		int id = -1;
		
		for (int i = 0; i < packets.size(); i++) {
			if (packets[i]->packetID == whichPacket){
				bDoesThisPacketExistAnywhere = true;
				id = i;
			}
		}
			
		if (bDoesThisPacketExistAnywhere){
			int startPos = whichPosition * goodChunkSize; 
			int endPos = MIN((whichPosition+1) * goodChunkSize, nBytesOfNormalUDP); 
			int length = endPos - startPos;
			packets[id]->addChunk(whichPosition, data, length);
						
		} else {
			chunkedPacket *	CPTemp;
			CPTemp = new chunkedPacket();
			CPTemp->setup(nChunks,  goodChunkSize);
			CPTemp->packetID = whichPacket;
			packets.push_back(CPTemp);
			int startPos = whichPosition * goodChunkSize; 
			int endPos = MIN((whichPosition+1) * goodChunkSize, nBytesOfNormalUDP); 
			int length = endPos - startPos;
			packets[packets.size()-1]->addChunk(whichPosition, data, length);
		}
	}
	
	// try to find the "freshest" full packet....
	void update( ){
		
		
		// if it's been a second without good packets, reset our newest data checks.  something is BAD !
		
		if (ofGetElapsedTimef() - lastGoodPacketTime > 1){
			newestData = -1;		// all is ok. 
		}
		
		
		bool bAnyFullPackets = false;
		long long largetsIdOfFullPacket = -1;
		int id = -1;
		
		for (int i = 0; i < packets.size(); i++) {
			if (packets[i]->bAmIFull()){
				
				lastGoodPacketTime = ofGetElapsedTimef();
				
				if ((packets[i]->packetID >= largetsIdOfFullPacket)){
					largetsIdOfFullPacket = 	packets[i]->packetID;
					newestData = largetsIdOfFullPacket;
					id = i;
				}
				bAnyFullPackets = true;
			}
		}
		

		
		if (bAnyFullPackets == true && id >= 0){
			
			// NOW do two things: 
			// a) get out the packet
			// b) get any OLDER packets out. 
			
			int pos = 0;
			for (int i = 0; i < nChunks; i++){
				int startPos = i * goodChunkSize; 
				int endPos = MIN((i+1) * goodChunkSize, nBytesOfNormalUDP); 
				int thisLength = endPos - startPos;
				memcpy(latestDataPacket + startPos, packets[id]->chunks[i].data, thisLength);
			}
			
			
			/*
			 // memory deallocation here !! please....
			 */
			
			std::vector<chunkedPacket *>::iterator iter = packets.begin();
			while (iter != packets.end())
			{
				if ((*iter)->packetID <= largetsIdOfFullPacket)
				{
					
					// seems to work!
					//printf("removing a packet with ID  %i \n", (*iter)->packetID);
					delete (*iter);	// this should !!! free memory.  please check
					
					iter = packets.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			 
			 
			//packets.erase( remove_if( packets.begin(), packets.end(), isLessThen ), packets.end() );
		} else {
			
		}

		// b) remove older packets
		
		
	}
	
	
};