package bankersAlgorithm;
import java.util.*;
import java.io.*;

public class Part1{
	private int n; //number of threads (np) 
    private int m; //number of resources (nr)
    private int avail[][];
    private int max[][];
    private int allocate[][];
    private int need[][];
    String filePath;
    
    private void initialize() throws FileNotFoundException{
     Scanner scan=new Scanner(System.in); //create new scanner
     System.out.println("please enter an input file path"); //prompt which file
     String file = scan.nextLine(); //get file path 
     System.out.println("please enter an output file path");
     filePath = scan.nextLine();
     scan.close();
     Scanner scan1 = new Scanner(new File(file)); //create scanner for file
     m=scan1.nextInt();  //read in no. of resources
     n=scan1.nextInt();  // read in no. of process
     need=new int[n][m];  //initialize arrays
     max=new int[n][m];
     allocate=new int[n][m];
     avail=new int[1][m];
     
     
     //read in available amount of each resource
     for(int i = 0;i<m; i++){ 
    	 avail[0][i] = scan1.nextInt(); //avail matrix 
     }
     
     //read in maximum integers describing maximum required resource for a process
     for(int i=0;i<n;i++){ //for each resource
         for(int j=0;j<m;j++){ //for each process
        	 max[i][j]=scan1.nextInt(); 
         }//max matrix
     }
     
   //read in initial allocation of resources to each process
     for(int f=0;f<n;f++){
          for(int g=0;g<m;g++){
        	//System.out.println(scan1.nextInt());
        	//System.out.println("n:"+ n + " m:" + m + " f:" + f + " g:" + g);
        	allocate[f][g]=scan1.nextInt();
          }//allocation matrix
     }
     scan1.close();
    }
    
    private int[][] calcNeed(){ //calculate need matrix based on inputs
       for(int i=0;i<n;i++)
         for(int j=0;j<m;j++)  
          need[i][j]=max[i][j]-allocate[i][j]; 
       return need;
    }
 
    private boolean check(int i){
       //check to see if all resources for all threads can be allocated
       for(int j=0;j<m;j++) 
       if(avail[0][j]<need[i][j]){
          return false;
       }
       return true;
    }

    public void SafetyCheck() throws IOException{
       initialize();
       calcNeed();
       boolean safe[]=new boolean[n]; //an array where each index indicates if thread has been allocated
       boolean allocated; //if it can be allocated or not
       int j=0;

       while(j<n){  //until all process allocated
       allocated=false;
       for(int i=0;i<n;i++) //for each thread
        if(!safe[i] && check(i)){  //if thread isn't allocated but can be
        	for(int k=0;k<m;k++){ //for each resource
        		avail[0][k]=avail[0][k]-need[i][k]+max[i][k]; //allocate the resource
        	}
        	//System.out.println("Allocated process : "+i);
        	allocated=safe[i]=true; //indicate that the thread has successfully been allocated
            j++;
        }
        if(!allocated){ 
        	break;  //if a thread couldn't be allocated, end loop
        }
       }
       if(j==n){  //if all processes are allocated
        System.out.println("Safe State");
        output(0);
       }
       else{
        System.out.println("Unsafe State");
        output(1);
       }
    }
    
    public void output(int n) throws IOException{
		PrintWriter file = new PrintWriter(new FileWriter(filePath));//make new file
		if(n == 0){
			file.print("Safe State");
		}
		if(n == 1){
			file.print("Unsafe State");
		}
		file.close();
    }
    
    public static void main(String[] args) throws IOException {
       new Part1().SafetyCheck();
    }
}