
extern int TabSig[];
extern int TabCos[];
extern int TabSin[];
// notre tableau commen�ant � 0, on prend la case 16 dans le tableau pour k =17
//r�sultat est dans r1
int M2(int,int*);

int main(){
	int resultat[64];
	int k,j;
	for(k=0;k<64;k++){
		j=k; 
		resultat[j]=M2(k,TabSig);
		k=j;
	}
	return 0;
}
