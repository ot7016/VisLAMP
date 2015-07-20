readmatrix <-function(infile,out){
	x <- read.table(infile,header = T)
	x2 <- x[,1:8]
	xscl <- scale(x2)[,] 
	m <- data.matrix(xscl)
	#本当はcategorical data は除くべきかもしれない
	k <- kmeans(t(m),2)
	
	write.csv(k$cluster,paste(out,"-cluster.csv",sep=""),quote =FALSE,row.names =FALSE)
	x3 <-data.frame(x2[,names(k$cluster[k$cluster== 1])],x2[,names(k$cluster[k$cluster== 2])])
	write.csv(x3,paste(out,".csv",sep=""),quote =FALSE,row.names = FALSE)
	kxscl <-data.frame(xscl[,names(k$cluster[k$cluster== 1])],xscl[,names(k$cluster[k$cluster== 2])])
	write.csv(kxscl,paste(out,"-scl.csv",sep = ""),quote =FALSE,row.names = FALSE)
	e <- eigen(crossprod(data.matrix(kxscl)),TRUE)
	write.csv(t(e$values),paste(out,"-eigen.csv",sep = ""),quote=FALSE,row.names=FALSE)
	
	
}
readmatrix3<-function(infile,out){
	t <- read.table(infile,header = T)
    t2 <- t[,1:8]   #現在 cars専用状態なのであとで対処
	tscl <- scale(t2)[,]
    dim <-ncol(t2)
    m <- rbind(data.matric(tscl),diag(dim))
	D0 <-dist(m,method ="euclidean",upper =TRUE) #距離オブジェクト作成
    D <-- as.matrix(D0) #距離行列に
	#cmdscale(D,k?,eig = TRUE)
	num <- nrow(m)
	
	dij <-sum(D^2)/num^2
	di <- apply(D,1,powsum)/num
	B1 <-  apply(D^2,1,minus,di)
	B2 <-  apply(B1,2,minus,di)
	B <- -( B2 + dij/num^2)/2  #Bij終了 young-holderで実装したほうがよかったかも..
	e <- eigen(B,TRUE)
	write.csv(t(e$values),paste(out,"-evalue.csv",sep = ""),quote=FALSE,row.names=FALSE)
	write.csv(t(e$vectors),paste(out,"-evector.csv",sep = ""),quote=FALSE,row.names=FALSE)
	lamh <- e$values[e$values>0]
	dim <- length(lamh)
	X <- apply(e$vectors[,1:dim],1,mul,sqrt(lamh))
	write.csv(t(X),paste(out,"-cood.csv",sep = ""),quote=FALSE,row.names=FALSE)
	  }
	powsum <- function(a){
		sum(a^2)
	}
	minus <- function(a,b){
		a-b
	}
mul <- function(a,b){
    a*b
}