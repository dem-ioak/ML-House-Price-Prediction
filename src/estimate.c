#include <stdio.h>
#include <stdlib.h>

double **multiply_matrix(double **a, double **b, int m, int n, int p, int q)
{
    if (n != p)
    {
        printf("Incompatible Matrices");
        exit(EXIT_FAILURE);
    }

    double **res = malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++)
    {
        res[i] = malloc(q * sizeof(double));
        for (int j = 0; j < q; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < p; k++)
                res[i][j] += a[i][k] * b[k][j];
        }
    }
    return res;
}

double **transpose(double **a, int r, int c)
{
    double **res = malloc(c * sizeof(double *));
    for (int i = 0; i < c; i++)
    {
        res[i] = malloc(r * sizeof(double));
        for (int j = 0; j < r; j++)
            res[i][j] = a[j][i];
        
    }
    return res;
}

double **invert(double **M, int r)
{
    // Create Identity Matrix and Deep Copy of A
    double **N = malloc(r * sizeof(double *)); // Identity Matrix
    for (int i = 0; i < r; i++)
    {
        N[i] = malloc(r * sizeof(double));
        for (int j = 0; j < r; j++)
            N[i][j] = i == j ? 1 : 0;
    }

    for (int p = 0; p < r; p++)
    {
        double val = M[p][p];
        if (val == 0)
        {
            printf("Division by Zero Error");
            exit(EXIT_FAILURE);
        }

        // Divide Row
        for (int i = 0; i < r; i++)
        {
            M[p][i] /= val;
            N[p][i] /= val;
        }
        for (int i = p + 1; i < r; i++)
        {
            val = M[i][p];
            // Multiply Each Row
            for (int j = 0; j < r; j++)
            {
                M[i][j] -= (M[p][j] * val);
                N[i][j] -= (N[p][j] * val);
            }
        }
    }

    for (int p = r - 1; p >= 0; p--)
    {
        for (int i = p - 1; i >= 0; i--)
        {
            double val = M[i][p];
            for (int j = 0; j < r; j++)
            {

                M[i][j] -= (M[p][j] * val);
                N[i][j] -= (N[p][j] * val);
            }
        }
    }
    return N;
}

void freeMatrix(double **matrix, int rows)
{
    for (int i = 0; i < rows; i++)
        free(matrix[i]);
    free(matrix);
}

int main(int argc, char *argv[])
{
    FILE *ptr = fopen(argv[1], "r");
    if (ptr == NULL)
        return EXIT_FAILURE;

    int r, c;
    char *w = malloc(24 * sizeof(char));

    fscanf(ptr, "%s", w);
    fscanf(ptr, "%d", &c);
    fscanf(ptr, "%d", &r);
    c++;

    // Allocate X and Y
    double **x = malloc(r * sizeof(double *));
    double **y = malloc(r * sizeof(double *));

    for (int i = 0; i < r; i++)
    {
        x[i] = malloc(c * sizeof(double));
        y[i] = malloc(sizeof(double));
        for (int j = 0; j <= c; j++)
        {
            // Col 0 of X
            if (j == 0)
            {
                x[i][j] = 1;
                continue;
            }

            // Read in prices of houses to Matrix Y
            if (j == c)
            {
                fscanf(ptr, "%lf", &y[i][0]);
                continue;
            }
            // Read in attributes of houses to Matrix X
            fscanf(ptr, "%lf", &x[i][j]);
        }
    }

    double **x_transpose = transpose(x, r, c);                                     // X^T
    double **multiplied = multiply_matrix(x_transpose, x, c, r, r, c);             // (X^T * X)
    double **inverted = invert(multiplied, c);                                     // (X^T * X)^-1
    double **second_multiply = multiply_matrix(inverted, x_transpose, c, c, c, r); // (X^T  * X)^-1 * X^T
    double **result = multiply_matrix(second_multiply, y, c, r, r, 1);             // (X^T  * X)^-1 * X^T * Y

    // Free all allocated matrices besides the result
    freeMatrix(x, r);               // 7x5
    freeMatrix(y, r);               // 7x1
    freeMatrix(x_transpose, c);     // 5x7
    freeMatrix(multiplied, c);      // 7x7
    freeMatrix(inverted, c);        // 7x7
    freeMatrix(second_multiply, c); // 5x7

    ptr = fopen(argv[2], "r");
    fscanf(ptr, "%s", w);
    fscanf(ptr, "%d", &c);
    fscanf(ptr, "%d", &r);

    // Read in data and compute X`W = Y
    double temp;
    int count;
    for (int i = 0; i < r; i++)
    {
        count = 0;
        double val = result[0][count++];
        for (int j = 0; j < c; j++)
        {
            fscanf(ptr, "%lf", &temp);
            val += result[count++][0] * temp;
        }
        printf("%.0f\n", val);
    }

    freeMatrix(result, c + 1);
    free(w);
    return EXIT_SUCCESS;
}