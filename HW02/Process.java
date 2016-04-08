public class Process{

  // overview: resource states illustrated through arrrays.
  protected int initial_allocation_array[];
  protected int maximum_required_array[];
  protected int need[];

  // constructor.
  Process(int n_resources){
    initial_allocation_array = new int[n_resources]; // an array of initial counts for how many of each resource
    maximum_required_array = new int[n_resources];
    need = new int[n_resources];
  }

}
