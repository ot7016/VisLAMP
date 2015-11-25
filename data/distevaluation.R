
# もともとの 高次元の座標と　高次元ベクトル、固有値を使った 2次元座標との間でどの程度距離に差があるのかを計算
distevaluation <- function(num,dim,dir1){
	# ファイルの場所とバイナリモードの読み込みの指定
	to.read = file(paste(dir1,"-cood.dat",sep = ""),"rb")
	# 読み込み方の指定をする
	cd <- readBin(to.read,double(),n=num* dim,endian = "little")
	cood <- matrix(cd,nrow = num, ncol = dim)
	close(to.read)
	to.read = file(paste(dir2,"-projection/projection.dat",sep= ""),"rb")
	# 読み込み方の指定をする
	prj <- readBin(to.read,double(),n=dim*2,endian = "little");
	close(to.read)
	projection<- matrix(prj,nrow = dim,ncol = num)
	#行列の形に

	#問題は距離の分布をどう調べるか
	#AGIとPCPの距離を変数としたときの相関係数を求めればよいのか?
	# PCPはマンハッタン距離でいいはず　(あとで要確認)

    pcpdis <- dist(cood,method = "manhattan")	
    pcpv <- as.vector(pcpdis)

	# i1 i2  成分がsum ((Ai1j - Ai2j)^2 )
	prj <- readBin(to.read,double(),n=dim*2,endian = "little");
	projection<- matrix(prj,nrow = dim,ncol = num)
	B <- cood %*% projection
	agidis <- dist(cood)
	agiv <- as.vector(pcpdis)
	cor(pcpv,agiv)
}
calcarsdist<- function(){
	distevaluation(398,8,"cars/cars")
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
	projection<- matrix(prj,nrow = dim,ncol = num)
	B <- cood %*% projection
	dis <- dist(B)
}


