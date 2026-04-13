package PO;

public class main {

	public static boolean encontrarn(int n, int[] array) {
	
				
		for(int i = 0; i < array.length; i++) {
			if(array[i] == n) return true;
		}
		
		return false;
		};


	public main() {
		
		int[] b = {1,2,3,4,5} ;
		
	System.out.println("resultado" + encontrarn(2, b));
	}
}
