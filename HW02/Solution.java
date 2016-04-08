import static java.lang.System.out;
import java.lang.*;
import java.util.*;
import java.io.*;

// HW02PT1 Solution.
public class Solution{


  // overview: main driver class.
  public static void main(String[] args) {  new Solution().exec();  }

  // overview: accepts filename relative to the cwd of the java program.
  public List<String> read_file(String path){
    List<String> list = new ArrayList<String>();
    File file = new File(path);
    BufferedReader reader = null;

    try {
      reader = new BufferedReader(new FileReader(file));
      String line = null;

      while ((line = reader.readLine()) != null) {
        list.add(line);
      }

    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();

    } finally {
      try {
        if (reader != null) {
          reader.close();
        }
      } catch (IOException e) { /*do nothing.*/}
    }

    return list;
  }

  // overview: initializes the Banker’s Algorithm state.
  public Bankers_Algorithm init(List<String> lines){
    Bankers_Algorithm object = new Bankers_Algorithm();

    int n_resources = Integer.parseInt(lines.get(0).split(" ")[0]);
    int m_processes = Integer.parseInt(lines.get(0).split(" ")[1]);
    int last_m_lines = (lines.size() - m_processes);

    Process processes[] = new Process[m_processes];
    List<String> initial_allocation_array = lines.subList(last_m_lines, lines.size());
    List<String> maximum_required_array = lines.subList(2, 2 + m_processes);

    // overview: The available amount of each resource (n integers)
    String available_amounts_forN_resource[] = lines.get(1).split(" ");


    object.available_resource_units = new int[n_resources];
    object.n = n_resources;
    object.m = m_processes;

    // overview: do for each process P[i]
    for (int i = 0; i < m_processes; i++) {
      String resource_init_data[] = initial_allocation_array.get(i).split(" ");
      String resource_maximum_required_data[] = maximum_required_array.get(i).split(" ");

      processes[i] = new Process(n_resources);
      for (int resource_id = 0; resource_id < n_resources; ++resource_id) {
        // overview: for each process, set how many units of resources (n) there are available
        // this count is accessible from any process P[i]
        object.available_resource_units[resource_id] =  Integer.parseInt(available_amounts_forN_resource[resource_id]);

        // overview: list of n ints describing the initial allocation of resources to each process
        processes[i].initial_allocation_array[resource_id] = Integer.parseInt(resource_init_data[resource_id]);

        // overview: list of n ints describing maximum required resources for a process
        processes[i].maximum_required_array[resource_id] = Integer.parseInt(resource_maximum_required_data[resource_id]);

        // overview: The difference between Max and Allocation.
        processes[i].need[resource_id] = processes[i].maximum_required_array[resource_id] - processes[i].initial_allocation_array[resource_id];
      }
    }
    object.set_processes(processes);
    return object;
  }

  // overview: print statements of the parsed data
  private void debug(Bankers_Algorithm object){
    for (Process p : object.processes) {
      out.printf("\nprocesses: \n");
      out.printf("resource array, available: %s (global value)\n" , Arrays.toString(object.available_resource_units));
      out.printf("resource array, maximum: %s\n" , Arrays.toString(p.maximum_required_array));
      out.printf("resource array, allocation: %s\n" , Arrays.toString(p.initial_allocation_array));
      out.printf("resource array, need: %s\n" , Arrays.toString(p.need));
    }
  }

  // overview: tally the total amount of resources.
  private int total_resource_count(Bankers_Algorithm x){
    int total_resources_available = 0;
    for (int i : x.available_resource_units) { total_resources_available += i; }

    for (Process process : x.processes) {
      for (int i : process.initial_allocation_array) {
        total_resources_available += i;
      }
    }

    return total_resources_available;
  }

  // overview: run Banker's Algorithm on all provided .txt files in dir.
  private void exec(){
    for (int i = 0; i < 4; ++i) {
      String file = String.format("discussion-10-examples/example%s.txt", Integer.toString(i+1));
      List<String> lines = new Solution().read_file(file);
      Bankers_Algorithm part_one = new Solution().init(lines);

      // new Solution().debug(part_one);
      String output = (part_one.safety_check())?"Safe State":"Unsafe State";
      out.printf("%s [%s]\n", file, output);
    }
  }




  // overview: private inner class known and accessible only to the Solution class.
  private static class Bankers_Algorithm{
    private Process processes[];
    private int available_resource_units[];
    private int n, m;

    // setter.
    private void set_processes(Process ...p){
      this.processes = p;
    }

    // overview: used to determine if the given system state is safe
    // If the state described in the file is safe, print “Safe State” and a safe sequence
    // If the given state is not safe, print “Unsafe State”
    private boolean safety_check(){
      // 1.
      int WORK[] = new int[n];
      boolean FINISH[] = new boolean[m];

      // WORK = this.available_resource_units;
      for (boolean i : FINISH) {   i = false; }

      //2.
      // Find an index i // find a process such that:
      for (int i = 0; i < m /*process array length*/; ++i) {

        for (int resource_id = 0; resource_id < n ; resource_id++) {
          // if(FINISH[i] == false && processes[i].need[resource_id] <= available_resource_units[resource_id]){
          if(FINISH[i] == false && resource_request_check(i, resource_id)){



            // 3.
            // if true, then Update Work and Finish

            WORK[resource_id] = available_resource_units[resource_id] = available_resource_units[resource_id] + processes[i].initial_allocation_array[resource_id];
            FINISH[i] = true;
            // out.println("it is now true");
          }

          // jump back to the start of the outter loop.
          // find new i index.
          break;
        }

      } // end of outter for loop.

      // 4.
      for (boolean j : FINISH) { if(j == false){ return false;} }
      return true;
    }

    // overview: which grants or rejects resource requests based on the
    // safety of the resulting state.
    private boolean resource_request_check(int i, int resource_id){
      // 2.
      int REQUEST, NEED, AVAILABLE, ALLOCATION;

      REQUEST = processes[i].maximum_required_array[resource_id]; // the requested resource.
      NEED = processes[i].need[resource_id];
      AVAILABLE = available_resource_units[resource_id];
      ALLOCATION = processes[i].initial_allocation_array[resource_id];

      out.printf("request: %d, need: %d\n", REQUEST, NEED);
      if(REQUEST > NEED && REQUEST > AVAILABLE){
        return false;
      }

      AVAILABLE = available_resource_units[resource_id] = AVAILABLE - REQUEST;
      ALLOCATION = processes[i].initial_allocation_array[resource_id] = ALLOCATION + REQUEST;

      return true;
    }
  }

}
