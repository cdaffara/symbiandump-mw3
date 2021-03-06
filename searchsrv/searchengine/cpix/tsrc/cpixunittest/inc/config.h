/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#ifndef CONFIG_H_
#define CONFIG_H_

#include <indevicecfg.h>

//
// Corpus locations
//

#ifdef __WINS__ 
#define RESOURCE_PATH 				"c:\\data\\cpixunittestcorpus\\resource\\"
#define CORPUS_PATH 				"c:\\data\\cpixunittestcorpus"
#define LCORPUS_PATH 				L"c:\\data\\cpixunittestcorpus"
#else // __ARMv5__, assume memory card
#define RESOURCE_PATH 				"e:\\data\\cpixunittestcorpus\\resource\\"
#define CORPUS_PATH 				"e:\\data\\cpixunittestcorpus"
#define LCORPUS_PATH 				L"e:\\data\\cpixunittestcorpus"
#endif

#define SMS_TEST_CORPUS_PATH 		CORPUS_PATH "\\sms\\SMS_corpus.txt"
#define STEM_TEST_CORPUS_PATH  		CORPUS_PATH "\\stem"
#define JPG_TEST_CORPUS_PATH  		CORPUS_PATH "\\jpgs"
#define MAPS_TEST_CORPUS_PATH  		CORPUS_PATH "\\maps"
#define MEDIA_TEST_CORPUS_PATH  	CORPUS_PATH "\\media"
#define LOC_TEST_CORPUS_PATH  		CORPUS_PATH "\\loc"

#define LJPG_TEST_CORPUS_PATH  		LCORPUS_PATH L"\\jpgs"

// TODO: Obsolete these
#define FILE_TEST_CORPUS_PATH  		STEM_TEST_CORPUS_PATH
#define DEFAULT_TEST_CORPUS_PATH 	SMS_TEST_CORPUS_PATH

//
// Indexing constants
//

#define CPIX_FILE_IDXDB      "\\root\\file"

#define CPIXUNITTEST_DIRECTORY "c:\\data\\cpixunittest"

/**
 * Zero terminated array of file names
 */
extern const char* Mp3TestCorpus[];

#endif /*CONFIG_H_*/
