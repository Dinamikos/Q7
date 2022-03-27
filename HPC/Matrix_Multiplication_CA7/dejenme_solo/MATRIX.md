# Matrix Documntation

---

This is a full guide to get a deep knowledge about matrix manipulation in order to these libs, to create a std with all of the algorithms used for Matrix Multiplication algorithms.

Matrix are created by rows and columns, for several algorithms is the same value to mark n as the main size proposed for algorithm simplicity. But to ease the manipulation into different applications the rows and columns are posted into two different variables.

```c
  int r = 10, c = 10; // rows and columns

  // memory allocated with r and c for integer matrix
  int* matrix = malloc((r * c) * sizeof(int)); 
```

**Access to, i and j exchange to a mem allocated way**

```c
    //matrix[i][j]
    matrix[i * c + j] = 0;
```

**y function works only with n**

If your function require a n matrix with the same number i, please manipulate as default r and c, you can modify your code saying that n is equals to r or c.

# Matrix utils - mtxutils.h

---

mtxutils.h contains the following functions:

- randMatrix(): random matrix
- printMatrix(): print matrix
- writeMatrixF(): write a matrix in a file
- readMatrixF(): read a matrix from a file
- zeroMatrix(): set zero matrix

# Matrix Multiplication Operations - mtxmop.h

---

mtxmop.h contains the following functions:

- S_orgMM(): Sequential original Matrix Multiplication
- P_rowMM(): Parallel row based Matrix Multiplication
- P_colMM(): Parallel column based Matrix Multiplication
- P_blockMM(): Parallel block based Matrix Multiplication
- S_TriangularVMM(): Sequential Triangular Vertex Matrix Multiplication
- P_TriangularVMM(): Parallel Triangular Vertex Matrix Multiplication

- [ ] check if he size for matrix are correct