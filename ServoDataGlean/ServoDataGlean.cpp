// FOCAS_History.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Fwlib32.h"
#include <stdio.h>
#include <Windows.h>
#include <math.h>


int main(int argc, char *argv[])
{

	// Requires FANUC FOCAS2 *.dll Files

	// Reuse variables
	unsigned short fwlibHndl;
	int result = -1;

	// Default Connection Parameters
	int port = 8193;
	char ip[] = "192.168.82.12";
	int timeout = 10;
	char filename[] = "posvectr.csv";
	int numSamples = 2000;
	long sequenceNum = 0;
	int scopeChannels = 1;
	short programNumber = -1;
	int ch1Data = 0;
	int ch2Data = 0;

	// Command Line Arguments
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-I") == 0) { i += 1;  strcpy(ip, argv[i]); }
		else if (strcmp(argv[i], "-P") == 0) { i += 1; port = atoi(argv[i]); }
		else if (strcmp(argv[i], "-T") == 0) { i += 1; timeout = atoi(argv[i]); }
		else if (strcmp(argv[i], "-F") == 0) { i += 1; strcpy(filename, argv[i]); }
		else if (strcmp(argv[i], "-S") == 0) { i += 1; numSamples = atoi(argv[i]); }
		else if (strcmp(argv[i], "-N") == 0) { i += 1; sequenceNum = atoi(argv[i]); }
		else if (strcmp(argv[i], "-C") == 0) {i += 1; scopeChannels = atoi(argv[i]); }
		else if (strcmp(argv[i], "-O") == 0) { i += 1; programNumber = atoi(argv[i]); }
		else { printf("Illegal Command Line Argument: %s\n", argv[i]); return 1; }
	};

	// Create New File
	FILE * pFile;
	pFile = fopen(filename, "w");
	if (pFile == NULL)
	{
		printf("Unable to open file.");
		return 1;
	}

	// Validate the Number of Channels and write the header
	char header[256];
	switch (scopeChannels){
	case 1:
		strcpy(header, "Sample,Channel1\n");
		break;
	case 2:
		strcpy(header, "Sample,Channel1,Channel2\n");
		break;
	default:
		printf("Illegal number of channels, must be 1 or 2."); 
		return 1;
	}
	fprintf(pFile, header);

	// validate the numSamples is divisible by 8
	if ((numSamples % 8) != 0)
	{
		printf("The number of samples is not evenly divisible by 8.");
		return 1;
	}

	// validate the numSamples is within range
	switch (scopeChannels)
	{
	case 1:
		if ( 0 > numSamples || numSamples > 233008){
			printf("Single channel sample limit of 233008 is exceeded.");
			return 1;
		}
	case 2:
		if (0 > numSamples || numSamples > 123352){
			printf("Two channel sample limit of 123352 is exceeded.");
			return 1;
		}
	}

	// Declarations
	bool error = false;
	const int bufSize = 64;

	// Get Library handle number
	result = cnc_allclibhndl3(ip, port, 10, &fwlibHndl);
	if (result != 0) {
		printf("Could not connect to CNC: %d\n", result);
		return 1;
	}

	unsigned long read[4];
	result = cnc_rdcncid(fwlibHndl, read);

	// Always clear existing channel data
	result = cnc_sdtclrchnl(fwlibHndl);
	if (result != 0) {
		printf("Could not clear channel data: %d\n", result);
		return 1;
	}

	const int numChannels = scopeChannels * 2;
	IDBSDTCHAN channel[4];
	channel[0].type = ch1Data;
	channel[0].chno = 1;
	channel[0].axis = 1;
	channel[0].shift = 0;
	channel[1].type = ch1Data;
	channel[1].chno = 1;
	channel[1].axis = 1;
	channel[1].shift = 15;
	channel[2].type = ch2Data;
	channel[2].chno = 1;
	channel[2].axis = 2;
	channel[2].shift = 0;
	channel[3].type = ch2Data;
	channel[3].chno = 1;
	channel[3].axis = 2;
	channel[3].shift = 15;

	// Set the channel data
	result = cnc_sdtsetchnl(fwlibHndl, numChannels, numSamples, channel);
	if (result != 0) {
		printf("Could not set channel data: %d\n", result);
		return 1;
	}

	// Wait for the correct program if program number is specified
	ODBPRO odbpro;
	bool correctProgram = false;

	while (!correctProgram && programNumber != -1){
		result = cnc_rdprgnum(fwlibHndl, &odbpro);
		if (result != 0){
			printf("Could not read program number: %d\n", result);
			return 1;
		}
		if (programNumber == odbpro.data){
			correctProgram = true;
		}
	}


	// Start Sampling
	result = cnc_sdtstartsmpl(fwlibHndl, 1, sequenceNum);
	if (result != 0) {
		printf("Could not start sampling: %d\n", result);
		return 1;
	}

	// Start the read pointer at the first data (B)
	long readPointer = 0;

	// Setup auto origin to the middle of the range
	unsigned long originOffset1 = -1;
	unsigned long originOffset2 = -1;

	// Secure the buffer area (D)
	ODBSD odbsd[4];
	unsigned short * tbuf1 = (unsigned short*)malloc(sizeof(unsigned short[bufSize]));
	unsigned short * tbuf2 = (unsigned short*)malloc(sizeof(unsigned short[bufSize]));
	unsigned short * tbuf3 = (unsigned short*)malloc(sizeof(unsigned short[bufSize]));
	unsigned short * tbuf4 = (unsigned short*)malloc(sizeof(unsigned short[bufSize]));

	odbsd[0].chadata = tbuf1;
	odbsd[1].chadata = tbuf2;
	odbsd[2].chadata = tbuf3;
	odbsd[3].chadata = tbuf4;

	long tcount1 = 0;
	long tcount2 = 0;
	long tcount3 = 0;
	long tcount4 = 0;

	odbsd[0].count = &tcount1;
	odbsd[1].count = &tcount2;
	odbsd[2].count = &tcount3;
	odbsd[3].count = &tcount4;

	while (readPointer < numSamples && error != true)
	{

		// How many data points are left to be read? (C)
		long remainingQty = numSamples - readPointer;

		// Reset Buffer Read Completion Flag
		short completeFlag = -1;

		while (completeFlag == -1 && error != true){

			// Read of sampling data
			result = cnc_sdtreadsmpl(fwlibHndl, &completeFlag, bufSize, odbsd);
			if (result != 0) {
				printf("Could not read samples from buffer: %d\n", result);
				return 1;
			}

		} // Confirm the argument (stat)


		// The data of each channel is taken out
		for (int i = 0; i < (long)odbsd[0].count[0];i++){

			// Combine high and low order bytes for the full 4 byte long
			unsigned long axisPos1 = (((unsigned short)odbsd[1].chadata[i] & 0x7FFE) * powl(2, 15)) + ((unsigned short)odbsd[0].chadata[i]);
			if (originOffset1 == -1){ originOffset1 = powl(2, 30) - axisPos1; }
			unsigned long correctedAxisPos1 = axisPos1 + originOffset1;

			if (scopeChannels == 2)
			{
				unsigned long axisPos2 = (((unsigned short)odbsd[3].chadata[i] & 0x7FFE) * powl(2, 15)) + ((unsigned short)odbsd[2].chadata[i]);
				if (originOffset2 == -1){ originOffset2 = powl(2, 30) - axisPos2; }
				unsigned long correctedAxisPos2 = axisPos2 + originOffset2;
				fprintf(pFile, "%lu,%lu,%lu\n", (i + readPointer), correctedAxisPos1, correctedAxisPos2);
			}
			else {
				fprintf(pFile, "%lu,%lu\n", (i + readPointer), correctedAxisPos1);
			}


		}

			
		// Read pointer is repositioned by the number actually read
		readPointer = readPointer + odbsd[0].count[0];
		
	} // A <= B? .... Total Reading Number <= Actual Read? 

	// End of Sampling
	result = cnc_sdtendsmpl(fwlibHndl);
	if (result != 0) {
		printf("Could not properly end sampling: %d\n", result);
		return 1;
	}

	free(tbuf1);
	free(tbuf2);
	free(tbuf3);
	free(tbuf4);
	fclose(pFile);
	result = cnc_freelibhndl(fwlibHndl);

	return 0;
}

