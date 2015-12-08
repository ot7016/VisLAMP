
# もともとの 高次元の座標と　高次元ベクトル、固有値を使った 2次元座標との間でどの程度距離に差があるのかを計算
distevaluation <- function(num,dim,dir1,index){
	# ファイルの場所とバイナリモードの読み込みの指定
	read1 = file(paste(dir1,"-original.dat",sep = ""),"rb")
	# 読み込み方の指定をする
	cd <- readBin(read1,double(),n=num* dim,endian = "little")
	cood <- t(matrix(cd,nrow = dim, ncol = num))
	close(read1)
	#read2 = file(paste(dir1,"-projection/projection.dat",sep= ""),"rb")
	# 読み込み方の指定をする
	#prj <- readBin(read2,double(),n=dim * 2, endian = "little")
	#close(read2)
	#projection<- matrix(prj,nrow = dim,ncol = 2)
	#行列の形に

	#問題は距離の分布をどう調べるか
	#AGIとPCPの距離を変数としたときの相関係数を求めればよいのか?
	# PCPはマンハッタン距離でいいはず　(あとで要確認)

    pcpdis <- dist(cood,method = "manhattan")	
    pcpv <- as.vector(pcpdis)

	# i1 i2  成分がsum ((Ai1j - Ai2j)^2 )
	#B <- cood %*% projection
	read2 = file(paste(dir1,"-agicood/agicood_",index, ".dat",sep = ""),"rb")
	# 読み込み方の指定をする
	cd <- readBin(read1,double(),n=num* 2,endian = "little")
	B <- t(matrix(cd,nrow = 2, ncol = num))
	close(read2)


	agidis <- dist(B)
	agiv <- as.vector(agidis)
	cor(pcpv,agiv)
}
calcarsdist<- function(index){
	distevaluation(398,8,"cars/cars",index)
}
calbikedist<- function(index){
	distevaluation(731,14,"bike/bike",index)
}
calspidist<- function(index){
	distevaluation(133,50,"spid/spid",index)
}
calturiyedist <- function(index){
	distevaluation(5820,32,"turkiye/turkiye",index)
}


# PCPの距離行列を計算
calPCPdist <- function(num,dim,dir1){
	
	# 読み込み方の指定をする
	to.read = file(paste(dir1,"-cood.dat",sep = ""),"rb")
	cd <- readBin(to.read,double(),n=num* dim,endian = "little")
	close(to.read)
	cood <- matrix(cd,nrow = num, ncol = dim)
	# PCPはマンハッタン距離でいいはず　(あとで要確認)

    dis <- dist(cood,method = "manhattan")	
	# i1 i2  成分がsum ((Ai1j - Ai2j)^2 )

	#問題は距離の分布をどう調べるか
	#AGIとPCPの距離を変数としたときの相関係数を求めればよい
	return( dis)
}
calAGIdist <- function(num,dim,dir1){
	# ファイルの場所とバイナリモードの読み込みの指定
	
	to.read = file(paste(dir1,"-cood.dat",sep = ""),"rb")
	# 読み込み方の指定をする
	cd <- readBin(to.read,double(),n=num* dim,endian = "little")
	cood <- matrix(cd,nrow = num, ncol = dim)
	to.read = file(paste(dir1,"-projection/projection.dat",sep = ""),"rb")
	# 読み込み方の指定をする
	prj <- readBin(to.read,double(),n=dim*2,endian = "little");
	projection<- matrix(prj,nrow = dim,ncol = 2)
	B <- cood %*% projection
	dis <- dist(B)
}


