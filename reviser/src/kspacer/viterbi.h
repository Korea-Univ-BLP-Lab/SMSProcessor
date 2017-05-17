#ifndef _VITERBI_H_
#define _VITERBI_H_

extern void unigram_viterbi_search(char splitchar[][3], int total_time, int total_class, int *state_sequence);
extern void bigram_viterbi_search(char splitchar[][3], int total_time, int total_class, int *state_sequence);
extern void trigram_viterbi_search(char splitchar[][3], int total_time, int total_class, int *state_sequence);

#endif
