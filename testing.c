#include <stdio.h>
#include <string.h>


// Function to generate combinations
void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data)) {
    if (k == 0) {
        
        process_selection(b, k, data);
        return;
    }

    if (n < k) {
        return; 
    }

    
    b[k - 1] = a[n - 1];
    generate_selections(a, n - 1, k - 1, b, data, process_selection);

    
    generate_selections(a, n - 1, k, b, data, process_selection);
}


void printarray(int b[],int k , void*data){
    int*d = (int*)data;

    for (int i = 0; i < k; i++)
    {
        printf("%d ",b[i]);
        ++*d;
    }
    printf("\n");
    
}

void count_selections(int b[], int k, void *data)
{
    int *d = (int*)data;
    ++*d;
}
void generate_splits_helper(const char *Sour, const char *dictionary[], int nwords, char buf[], int Sor_In, int bufIndex, void *data, void (*process_split)(char buf[], void *data))
{
    if (Sor_In == strlen(Sour)) {
        buf[bufIndex] = '\0';
        process_split(buf, data);
        return;
    }

    int wordIndex;
    wordIndex = 0;
    char word[100];
    for (; Sor_In < strlen(Sour); Sor_In++) {
        word[wordIndex] = Sour[Sor_In];
        wordIndex+=1;
        word[wordIndex] = '\0';
        int i=0;
        while(i<nwords) {
            if (strcmp(word, dictionary[i]) == 0) {
                strcpy(buf + bufIndex, word);
                bufIndex += strlen(word);
                if (Sor_In < strlen(Sour)) {
                    buf[bufIndex] = ' ';
                    bufIndex+=1;
                }

                generate_splits_helper(Sour, dictionary, nwords, buf, Sor_In, bufIndex, data, process_split);

                bufIndex -= strlen(word);
                if(Sor_In >= strlen(Sour)){
                    continue;
                }
                else  {
                    bufIndex-=1;
                }
                
            }
            i++;
        }
    }
}

void generate_splits(const char *Sour, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data))
{
    int Sor_In = 0;
    int bufIndex = 0;
    generate_splits_helper(Sour, dictionary, nwords, buf, Sor_In, bufIndex, data, process_split);
}
int main(int argc, char const *argv[])
{

    int c = 0;
    int b[10];
    int a[] = { 2, 1, 6, 5 };
    int aa[] = { 1, 5, 3, 0, 1, 12, 4, 3, 6, 6 };
    generate_selections(a, 4, 2, b, &c, count_selections);
    printf("%d",c);
    return 0;
}