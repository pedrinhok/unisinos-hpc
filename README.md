# unisinos-pad

## install (macosx)
```
brew install open-mpi
```

## compile
```
mpicc -o matrix-product ./matrix-product.c
```

## run
```
mpirun -np 2 ./matrix-product
```
