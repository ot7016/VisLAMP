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
readmatrix2<-function(t2,out,thr){
	#tscl <- scale(t2)[,]
	tmin <- apply(t2,2,min)
	tmax <- apply(t2,2,max)
	trate <- tmax -tmin
	#trate <- tmax -tmin
	n <- nrow(t2)
	 dim <-ncol(t2)
	for(i in 1:n){
		ta <- t2[i,] - tmin
		ta2 <- ta/trate
	    t2 <- rbind(t2,ta2)
	} 
	tscl <- t2[(n+1):(2*n),]
   
    m <- rbind(data.matrix(tscl),diag(dim))
	D0 <-dist(m,method ="euclidean",upper =TRUE) #距離オブジェクト作成　categorical dataは同じ値なら 0 違うなら1とか
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
	lamh <- e$values[e$values>thr]
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
dee <- function(a,b){
	a/b
}


#thrは 閾値
readcars<- function(thr = 0){
    t <- read.table("cars-8/auto-mpg_withname.data",header = T)
    t2 <- t[,1:8]
    readmatrix2(t2,"cars-8/kcars",thr)
}
readspid <- function(thr = 0){
    t <- read.csv("spid2015/2015SocialprogressIndexData.csv",header = T)
    t2 <- t[1:133,3:18]  #全部表示すると潰れてしまうので今はこれで
    t1 <- t[1:133,1]
    write.csv(data.frame(t2,t1),"spid2015/spid-original.csv",quote = FALSE,row.names =FALSE)
    readmatrix2(t2,"spid2015/spid",thr)
}