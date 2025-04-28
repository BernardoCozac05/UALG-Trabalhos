#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 10000

// Structure to hold words and their counts
typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

// Function to compare two WordCount structures for sorting
int compare(const void *a, const void *b) {
    WordCount *wordCountA = (WordCount *)a;
    WordCount *wordCountB = (WordCount *)b;
    if (wordCountB->count != wordCountA->count) {
        return wordCountB->count - wordCountA->count; // Sort by count descending
    }
    return strcmp(wordCountA->word, wordCountB->word); // Sort alphabetically
}

// Function to check if a word is an article
int is_article(const char *word) {
    return (strcmp(word, "a") == 0 || strcmp(word, "o") == 0 ||
            strcmp(word, "as") == 0 || strcmp(word, "os") == 0 ||
            strcmp(word, "um") == 0 || strcmp(word, "uma") == 0 ||
            strcmp(word, "de") == 0 || strcmp(word, "que") == 0 ||
            strcmp(word, "e") == 0 || strcmp(word, "para") == 0 ||
            strcmp(word, "com") == 0 || strcmp(word, "da") == 0 ||
            strcmp(word, "do") == 0 || strcmp(word, "se") == 0 ||
            strcmp(word, "não") == 0 || strcmp(word, "mas") == 0 ||
            strcmp(word, "em") == 0 || strcmp(word, "por") == 0 ||
            strcmp(word, "na") == 0 || strcmp(word, "ao") == 0 ||
            strcmp(word, "dos") == 0 || strcmp(word, "das") == 0);
}

int main() {
    WordCount words[MAX_WORDS];
    int wordCount = 0;

    // Read from standard input
    char ch;
    char currentWord[MAX_WORD_LENGTH];
    int currentIndex = 0;

    while ((ch = getchar()) != EOF) {
        // Check if the character is alphabetic or an accented character
        if (isalpha(ch) || (ch >= 192 && ch <= 255)) { // ASCII range for accented characters
            currentWord[currentIndex++] = tolower(ch); // Convert to lowercase
        } else {
            if (currentIndex > 0) {
                currentWord[currentIndex] = '\0'; // Null-terminate the current word
                // Check if the word is an article
                if (!is_article(currentWord)) {
                    // Check if the word already exists in the array
                    int found = 0;
                    for (int i = 0; i < wordCount; i++) {
                        if (strcmp(words[i].word, currentWord) == 0) {
                            words[i].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found && wordCount < MAX_WORDS) {
                        strcpy(words[wordCount].word, currentWord);
                        words[wordCount].count = 1;
                        wordCount++;
                    }
                }
                currentIndex = 0; // Reset for the next word
            }
        }
    }

    // Handle the last word if the input does not end with a non-alphabetic character
    if (currentIndex > 0) {
        currentWord[currentIndex] = '\0';
        if (!is_article(currentWord)) {
            int found = 0;
            for (int i = 0; i < wordCount; i++) {
                if (strcmp(words[i].word, currentWord) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found && wordCount < MAX_WORDS) {
                strcpy(words[wordCount].word, currentWord);
                words[wordCount].count = 1;
                wordCount++;
            }
        }
    }

    // Sort the words by count and then alphabetically
    qsort(words, wordCount, sizeof(WordCount), compare);

    // Read the number of top occurrences to display
    int N;
    scanf("%d", &N); // Read the number of top occurrences

    // Print the top N results
    for (int i = 0; i < N && i < wordCount; i++) {
        printf("%d %s\n", words[i].count, words[i].word);
    }

    return EXIT_SUCCESS;
}