#pragma once
#include <stddef.h>

/*
	Class to provide a statistic to the N-Gram algorithm
	An ngram stat stores a pattern and the choice that was made immediately following that pattern.
	This is useful to the ngram in that it can then use the stats to determine the frequency a player makes choice X given
	a pattern P.
 */
class ngram_stat
{
private:
	char * pattern; 	//pointer to an array of char that is the pattern for this stat
	char choice;		//represents the choice immediately following the pattern
	size_t patLength;	//the length of the pattern
public:
	ngram_stat(char, char*, size_t);
	~ngram_stat();
	char * getPattern();
	char getChoice();
	char * getNextPattern();
};

