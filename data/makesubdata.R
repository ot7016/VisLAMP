#!/usr/bin/rscript
source("../data/calmatrix.R")
args <- commandArgs(trailingOnly = T)
dataname <- args[1]
num <- as.integer(args[2])
atr <-as.integer(args[3])
to.read = file(paste(dataname,"-original.dat",sep = ""), "rb")
cd <-readBin(to.read, double(), n = num* atr, endian = "little")
data <-t(matrix(cd,nrow = atr, ncol = num))
print(data[1,])
calpca(data,paste(dataname, sep = "") )