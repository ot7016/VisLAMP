calpca<- function(t,out){
	t[is.na(t) ] <- 0
	print("主成分分析")
	 pca <-prcomp(t,scale = TRUE)
	 # sdevは固有値の平方根
	 print("計算終了")
	 sevalue <- pca$sdev
	 evalue <- as.double(sevalue^2)
	 print("evalue出力")
	 wt = file(paste(out,"-evalue.dat",sep = ""),"wb")
	 writeBin(evalue,wt)
	 close(wt)
	 sevector<- pca$rotation
	 evector<-as.double(t(as.matrix(sevector)))
	 print("evector出力")
	 wt = file(paste(out,"-evector.dat",sep = ""),"wb")
	 writeBin(evector,wt)
	 close(wt)
	 #これが高次元座標
	 cood<- as.matrix(pca$x)
	 cood2<- as.double(t(cood))
	 print("高次元座標出力")
	 wt = file(paste(out,"-cood.dat",sep = ""),"wb")
	 writeBin(cood2,wt)
	 close(wt)
	# write.csv(pca$x,paste( out, "-cood.csv"),quote = FALSE,row.names = FALSE)
	 #距離行列
	 D0 <-dist(cood,upper =TRUE) #距離オブジェクト作成　categorical dataは同じ値なら 0 違うなら1とか
    D <- as.matrix(D0) #距離行列に
    D1 <- as.double(D)
    wt = file(paste(out,"-dist.dat",sep = ""),"wb")
	writeBin(D1,wt)
	close(wt)
}


readmatrix2<-function(t2,out,thr){
	#tscl <- scale(t2)[,]
	tmin <- apply(t2,2,min)
	tmax <- apply(t2,2,max)
	trate <- tmax -tmin
	n <- nrow(t2)
	 dim <-ncol(t2)
	for(i in 1:n){
		ta <- t2[i,] - tmin
		ta2 <- ta/trate
	    t2 <- rbind(t2,ta2)
	} 
	tscl <- t2[(n+1):(2*n),]
   
    m <- rbind(data.matrix(tscl),diag(dim))
	D0 <-dist(m,upper =TRUE)^2 #距離オブジェクト作成　categorical dataは同じ値なら 0 違うなら1とか
    D <- as.matrix(D0) #距離行列に
    #D1 <- as.double(D)
    #wt = file(paste(out,"-adjency.dat",sep = ""),"wb")
	#writeBin(D1,wt)
	#close(wt)
	num <- nrow(m)
	dij <-sum(D^2)/num^2
	di <- apply(D,1,powsum)/num
	B1 <-  apply(D^2,1,minus,di)
	B2 <-  apply(B1,2,minus,di)
	B <- -( B2 + dij/num^2)/2  #Bij終了 young-holderで実装したほうがよかったかも..
	e <- eigen(B,TRUE)
	lamh <- as.double(e$values[e$values>thr])
	dim <- length(lamh)
	veclam <- e$vectors[,1:dim]
	
	wt = file(paste(out,"-evalue.dat",sep = ""),"wb")
	writeBin(lamh,wt)
	close(wt)
	#wt = file(paste(out,"-evector.dat",sep = ""),"wb")
	#veclam2 <- as.double(as.matrix(veclam))
	#writeBin(veclam2,wt)
	#close(wt)

	X <- apply(veclam,1,mul,sqrt(lamh))
	X2 <- as.double(X)
	wt = file(paste(out,"-cood.dat",sep = ""),"wb")
	writeBin(X2,wt)
	close(wt)

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

readcarpca<-function(){
	t <- read.table("cars-pca/auto-mpg_withname.data",header = T)
    t1 <- t[,1:8]
	wt = file("cars-pca/kcars-original.dat","wb")
	t2 <- t(as.matrix(t1))
	t3 <- as.double(t2)
	writeBin(t3,wt)
	close(wt)
	write.csv(colnames(t)[1:8],"cars-pca/kcars-atrname.csv",quote = FALSE,row.names = FALSE)
	
	write.csv(t[,9],"cars/cars-name.csv",quote = FALSE,row.names = FALSE)
	 calpca(t1,"cars/cars")
}
readturkiyepca <- function(){
	t <- read.csv("turkiye-pca/turkiye-student-evaluation_R_Specific.csv",header = T)
	t2 <- t[,2:33]
	#t3 <- t[,1]
	write.csv(colnames(t)[2:33],"turkiye-pca/turkiye-atrname.csv",quote = FALSE,row.names = FALSE)
    write.csv(rownames(t),"turkiye-pca/turkiye-name.csv",quote = FALSE,row.names = FALSE)
	wt = file("turkiye/turkiye-original.dat","wb")
	t4 <- as.double(t(as.matrix(t2)))
	writeBin(t4,wt)
	close(wt)
	calpca(t2,"turkiye/turkiye")
}

readspidpca <- function(){
    t <- read.csv("spid-pca/2015SocialprogressIndexData.csv",header = T)
    t2 <- t[1:133,3:53]  #全部表示すると潰れてしまうので今はこれで
    t1 <- t[1:133,1]
    t3 <- as.double(t(as.matrix(t2)))
    write.csv(colnames(t)[3:53],"spid-pca/spid-atrname.csv",quote = FALSE,row.names = FALSE)
    write.csv(t1,"spid-pca/spid-name.csv",quote = FALSE,row.names = FALSE)
    wt = file("spid/spid-original.dat","wb")
    writeBin(t3,wt)
    close(wt)
    calpca(t2,"spid/spid")
}
readbikepca <- function(){
	t <- read.csv("bike-pca/day.csv",header = T)
    t2 <- t[,3:16]  #全部表示すると潰れてしまうので今はこれで
    t1 <- t[,2]
    t3 <- as.double(t(as.matrix(t2)))
    write.csv(colnames(t)[3:16],"bike-pca/bike-atrname.csv",quote = FALSE,row.names = FALSE)
    write.csv(t1,"bike-pca/bike-name.csv",quote = FALSE,row.names = FALSE)
    wt = file("bike-pca/bike-original.dat","wb")
    writeBin(t3,wt)
    close(wt)
    calpca(t2,"bike/bike")

}
#xmdvからダウンロードしたものを使うとき	
readxmdvpca <- function(str){
	dir <- paste(str,"/",str,sep = "")
	t2 <- read.table(paste(dir,".txt",sep = ""),header = T)
    t3 <- as.double(t(as.matrix(t2)))
    write.csv(colnames(t2),paste(dir,"-atrname.csv",sep = ""),quote = FALSE,row.names = FALSE)
    write.csv(rownames(t2),paste(dir,"-name.csv",sep = ""),quote = FALSE,row.names = FALSE)
    wt = file(paste(dir,"-original.dat",sep = ""),"wb")
    writeBin(t3,wt)
    close(wt)
    calpca(t2,dir)

}



readtenkipca <- function(){
	t <- read.csv("tenki/tenki.csv",header = T)
     #全部表示すると潰れてしまうので今はこれで
    t2 <- cbind(t[,2:5],t[,7],t[,9:10],t[12:13])
    t1 <- t[,1]
    t3 <- as.double(t(as.matrix(t2)))
    write.csv(colnames(t2),"tenki/tenki-atrname.csv",quote = FALSE,row.names = FALSE)
    write.csv(t1,"tenki/tenki-name.csv",quote = FALSE,row.names = FALSE)
    wt = file("tenki/tenki-original.dat","wb")
    writeBin(t3,wt)
    close(wt)
    calpca(t2,"tenki/tenki")

}



#thrは 閾値
readcars<- function(thr = 0){
    t <- read.table("cars-8/auto-mpg_withname.data",header = T)
    t1 <- t[,1:8]
	wt = file("cars-8/kcars-original.dat","wb")
	t2 <- t(as.matrix(t1))
	t3 <- as.double(t2)
	writeBin(t3,wt)
	close(wt)
	write.csv(colnames(t)[1:8],"cars-8/kcars-atrname.csv",quote = FALSE,row.names = FALSE)
	
	write.csv(t[,9],"cars-8/kcars-name.csv",quote = FALSE,row.names = FALSE)
	

    readmatrix2(t1,"cars-8/kcars",thr)
}
readspid <- function(thr = 0){
    t <- read.csv("spid2015/2015SocialprogressIndexData.csv",header = T)
    t2 <- t[1:133,3:53]  #全部表示すると潰れてしまうので今はこれで
    t1 <- t[1:133,1]
    t3 <- as.double(t(as.matrix(t2)))
    write.csv(colnames(t)[3:53],"spid2015/spid-atrname.csv",quote = FALSE,row.names = FALSE)
    write.csv(t1,"spid2015/spid-name.csv",quote = FALSE,row.names = FALSE)
    wt = file("spid2015/spid-original.dat","wb")
    writeBin(t3,wt)
    close(wt)
    readmatrix2(t2,"spid2015/spid",thr)
}
readturkiye <- function(thr = 0){
	t <- read.csv("turkiye-student/turkiye-student-evaluation_R_Specific.csv",header = T)
	t2 <- t[,2:33]
	t3 <- t[,1]
	write.csv(colnames(t)[2:33],"turkiye-student/turkiye-atrname.csv",quote = FALSE,row.names = FALSE)
    write.csv(t3,"turkiye-student/turkiye-name.csv",quote = FALSE,row.names = FALSE)
	wt = file("turkiye-student/turkiye-original.dat","wb")
	t4 <- as.double(t(as.matrix(t2)))
	writeBin(t4,wt)
	close(wt)
	readmatrix2(t2,"turkiye-student/turkiye",thr)
}
