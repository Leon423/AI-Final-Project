#include "pch.h"
#include "ngram_stat.h"
#include <stddef.h>
#include <iostream>

/*
	Constructor. Takes a choice, a pattern for that choice and the length of the pattern
 */
ngram_stat::ngram_stat(char choice, char * pattern, size_t patLength) {
	this->choice = choice;						//set choice to specified choice
	this->patLength = patLength;				//set patlength to specified length
	this->pattern = new char[patLength + 1];		//allocate a new array of char of size patLength
	for (int i = 0; i < patLength; i++)			//copy provided pattern to new array
		this->pattern[i] = pattern[i];
	this->pattern[patLength] = '\0';			//add a terminating null character to the end of the pattern
}

ngram_stat::~ngram_stat() {
	delete pattern;								//delete the pattern array on object deletion
}

char * ngram_stat::getPattern() {
	return this->pattern;						//return pointer to our pattern
}

char ngram_stat::getChoice() {
	return this->choice;						//return our choice
}

char * ngram_stat::getNextPattern() {			//return the pattern that is formed by the pattern starting at index 1 + the choice
	char * nextPat = new char[patLength + 1];		//allocate the next pattern array
	for (int i = 0; i < patLength - 1; i++)		//copy old pattern to new pattern
		nextPat[i] = this->pattern[i + 1];
	nextPat[patLength - 1] = this->choice;		//add choice to the end of the new pattern
	nextPat[patLength] = '\0';					//add terminating null char
	return nextPat;								//return pointer to new pattern array
}