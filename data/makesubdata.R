#!/bin/r
source("calmatrix.R")
args <- commandArgs(trailingOnly = T)
dataname <- args[1]
num <- args[2]
atr <- args[3]
to.read = file(paste(dataname,"-original.dat",sep = ""), "rb")
cd <-readbin(to.read, double(), n = num* atr, endian = "little")
data <-t(matrix(cd,nrow = num, ncol = atr))
calpca(data,paste(dataname, sep = "") )