#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1




/*
 * PDN Programming: Homework 1, Problem 1
 *
 * CS 4473
 *
 * Kyle Murphey
 *
 * 02/07/21
 *
 */




/* structure used for storing values from a matrix or vector */
typedef struct MatrixNode
{
    double val; // value to store
    struct MatrixNode* next; // location of next structure holding the value
} MatrixNode;



/*
 * deletes the nodes in the DLL (this function is very similar to delete functions that I've made in the past on my projects)
 *
 * @param:matrix = the head of the dynamically linked list to delete
 */
void deleteNodes(MatrixNode** matrix)
{
    MatrixNode *cur = *matrix; // the current node to free
    MatrixNode *temp; // temporary placeholder for the next node in line

    //loop through the DLL starting at the head, freeing up each node
    while(cur->next != NULL)
    {
        temp = cur->next;
        free(cur);
        cur = temp;
    }
    free(cur);
    *matrix = NULL;
}



/*
 * output the result DLL to the output file
 *
 * @param:resultVector = vector to output to the file
 * @param:outputFile = file to send the resulting vector to
 */
void outToFile(MatrixNode *resultVector, FILE *outputFile)
{
    //outputting the resulting vector to the output file
    while(resultVector->next != NULL)
    {
        fprintf(outputFile, "%f\n", resultVector->val);
        resultVector = resultVector->next;
    }
    fprintf(outputFile, "%f", resultVector->val);
}



/*
 * calculates the product of a matrix and vector and stores the resulting vector in a DLL (resultVector)
 *
 * @param:matrix = matrix DLL for multiplying
 * @param:vector = vector DLL for multiplying
 * @param:rows = number of rows in the matrix and vector
 * @param:cols = number of columns in the matrix
 * @param:outputFile = file to send the output to
 */
void findProduct(MatrixNode *matrix, MatrixNode *vector, int rows, int cols, FILE *outputFile)
{
    /* DLL holding the result of vector-matrix multiplication */
    MatrixNode* resultHead = NULL;
    resultHead = (MatrixNode*)malloc(sizeof(MatrixNode));
    memset(resultHead, 0, sizeof(MatrixNode));
    resultHead->val = 0.0;
    resultHead->next = NULL;


    MatrixNode * result = resultHead; // used to move through the resulting vector
    int started = FALSE; // flag for getting through the head of the list

    //loop through rows of the matrix
    for (int i = 0; i < rows; ++i)
    {
        //if false, this is the first run
        if (started == FALSE)
        {
            started = TRUE;
        }
        //else we want to allocate room for the next value in the resulting vector
        else
        {
            result->next = (MatrixNode*)malloc(sizeof(MatrixNode));
            memset(result->next, 0, sizeof(MatrixNode));
            result = result->next;
        }

        MatrixNode *vectorHead = vector; // location of the vector DLL head for the input vector file

        //loop through the matrix and vector
        for (int j = 0; j < cols; ++j)
        {
            result->val += matrix->val * vectorHead->val; // summing the products produced by vector-matrix multiplication
            matrix = matrix->next;
            vectorHead = vectorHead->next;
        }
        result->next = NULL;
        /* current row completed, looping down to the next one */
    }
    outToFile(resultHead, outputFile); // send the result to the output file
    deleteNodes(&resultHead); // free up memory
}



/*
 * parses the vector file and stores it into a vector DLL (vectorValueList)
 *
 * @param:vectorFile = file to read the vector from
 * @param:vectorValueList = the DLL used to hold the vector values
 */
void parseVector(FILE *vectorFile, MatrixNode *vectorValueList)
{
    MatrixNode *prevToken = NULL;
    prevToken = vectorValueList; // creating a temporary MatrixNode and setting it to the value of the head
    int createdHead = 0; // flag for head creation of DLL
    char* lineBuf = (char*)malloc(sizeof(char) * 1000); // enough room to store entire line including whitespace
    memset(lineBuf, 0, sizeof(*lineBuf)); // setting the buffer to values of 0

    // checking for mem error while allocating
    if (lineBuf == NULL)
    {
        printf("Error");
        exit(1);
    }
    //loop through the file line by line
    while (fgets(lineBuf, sizeof(char)*1000, vectorFile))
    {
        char *endptr; // memory used in string to double conversions

        if (createdHead == FALSE)
        {
            vectorValueList->val = strtod(lineBuf, &endptr);
            createdHead = TRUE; // head of DLL created
        }
        else
        {
            prevToken->next = (MatrixNode*)malloc(sizeof(MatrixNode)); // allocate room for next value

            MatrixNode *nextToken = NULL; // next token to store
            nextToken = prevToken->next;
            nextToken->val = strtod(lineBuf, &endptr);
            nextToken->next = NULL;

            prevToken = nextToken; // setting the previous node pointer (prevToken) to the current one (nextToken)
        }
    }
    free(lineBuf); // free up allocation
}



/*
 * parses a given matrix from a file and stores it into a DLL (matrixValueList)
 *
 * @param:matrixFile = file containing the matrix to be parsed
 * @param:matrixValueList = the DLL used to store the matrix values
 */
void parseMatrix(FILE *matrixFile, MatrixNode *matrixValueList)
{
    MatrixNode *prevToken = NULL;
    prevToken = matrixValueList; // creating a temporary MatrixNode and setting it to the value of the head
    int createdHead = 0; // flag for head creation of DLL
    char* lineBuf = (char*)malloc(sizeof(char) * 500000); // enough room to store entire line including whitespace
    memset(lineBuf, 0, sizeof(*lineBuf)); // setting the buffer to values of 0

    // checking for mem error while allocating
    if (lineBuf == NULL)
    {
        printf("Error");
        exit(1);
    }
    //loop through the file line by line
    while (fgets(lineBuf, sizeof(char)*500000, matrixFile))
    {
        char *endptr; // memory used in string to double conversions
        char *val = strtok(lineBuf, ","); // string used for tokenizing input line

        //loop through the line, tokenizing at each ',' and storing the value in val
        while (val)
        {
            //check DLL head creation flag
            if (createdHead == FALSE)
            {
                matrixValueList->val = strtod(val, &endptr);
                createdHead = TRUE; // head of DLL created
            }
            //head already created
            else
            {
                prevToken->next = (MatrixNode *) malloc(sizeof(MatrixNode)); // allocating space for the next MatrixNode
                memset(prevToken->next, 0, sizeof(MatrixNode));

                MatrixNode *nextToken = NULL; // next val to store
                nextToken = prevToken->next;
                nextToken->val = strtod(val, &endptr);
                nextToken->next = NULL;

                prevToken = nextToken; // setting the previous node pointer (prevToken) to the current one (nextToken)
            }
            val = strtok(NULL, ","); // continue tokenizing lineBuf
        }
    }
    free(lineBuf); // free up allocated memory
}



/*
 * parses the files and creates the DLLs for multiplication. Outputs to a file when done.
 *
 * @param:inputFile1 = matrix input file
 * @param:inputFile2 = vector input file
 * @param:outputFile = output file
 * @param:n_row1 = number of rows in the matrix
 * @param:n_col1 = number of columns in the matrix
 */
void calculate(FILE *inputFile1, FILE *inputFile2, FILE *outputFile, int n_row1, int n_col1)
{
    /* DLL holding the matrix values from csv */
    MatrixNode* matrixValueList = NULL;
    matrixValueList = (MatrixNode*)malloc(sizeof(MatrixNode));
    memset(matrixValueList, 0, sizeof(MatrixNode));
    matrixValueList->val = 0.0;
    matrixValueList->next = NULL;

    /* DLL holding the matrix values from csv */
    MatrixNode* vectorValueList = NULL;
    vectorValueList = (MatrixNode*)malloc(sizeof(MatrixNode));
    memset(vectorValueList, 0, sizeof(MatrixNode));
    vectorValueList->val = 0.0;
    vectorValueList->next = NULL;


    parseMatrix(inputFile1, matrixValueList); // parse the input matrix to a DLL
    parseVector(inputFile2, vectorValueList); // parse the input vector to a DLL
    findProduct(matrixValueList, vectorValueList, n_row1, n_col1, outputFile); // multiply the matrix and vector for resulting vector

    /* free up all of the allocated memory in the DLLs */
    deleteNodes(&matrixValueList);
    deleteNodes(&vectorValueList);

    /* free allocated memory */
    free(matrixValueList);
    free(vectorValueList);
}



/*
 * main function
 *
 * reads in parameters from the CLI and performs vector matrix multiplication,
 * displaying the results to an output file
 */
int main (int argc, char *argv[])
{
    //checks if we inputted enough parameters
    if( argc != 7)
    {
        printf("USE LIKE THIS: serial_mult_mat_vec file_1.csv n_row_1 n_col_1 file_2.csv n_row_2 outputfile.csv \n");
        return EXIT_FAILURE;
    }


    FILE *inputFile1; // matrix file
    FILE *inputFile2; // vector file
    FILE *outputFile; // output file

    char* p1; // end char for n_row1
    char* p2; // end char for n_col1
    char* p3; // end char for n_row2

    int n_row1 = strtol(argv[2], &p1, 10 ); // store the number of rows for matrix
    int n_col1 = strtol(argv[3], &p2, 10 ); // store the number of columns for matrix
    int n_row2 = strtol(argv[5], &p3, 10 ); // store the number of rows for vector

    //checks to see if matrix and vector can be multiplied together
    if(n_col1 != n_row2)
    {
        printf("INCOMPATIBLE MATRIX AND VECTOR SIZES\n");
        return EXIT_FAILURE;
    }

    inputFile1 = fopen(argv[1], "r"); // open the matrix file for reading
    inputFile2 = fopen(argv[4], "r"); // open vector file for reading
    outputFile = fopen(argv[6], "w"); // open output file for writing

    calculate(inputFile1, inputFile2, outputFile, n_row1, n_col1); // calculate the product and output to the file

    /* close all the files */
    fclose (inputFile1);
    fclose (inputFile2);
    fclose (outputFile);

    return 0;
}