var fs = require("fs") // file system object
var resources = 'resources/' // a string used to append to a substring of the cwd path
var curr_folder = process.cwd().slice(0, -3)  + resources // a substring of the cwd path

// package module for external/modular/future use.
module.exports = {


  /* overview: executes the parse script, begins by locating and returning
    an array of strings of trace filename paths

    @params: callback: function which recieves the returned array of strings
    returns an array of files in the cwd to callback
  */

  exec:
  function (callback){
      fs.readdir(curr_folder ,function (err, files){
        if (err) throw err;

        callback(files)
      })
  },


  /* overview: builds a json from the contents of a trace .txt file.
    the json defines a job data structure.

    @params: file: path to a trace .txt file
    @params: callback: function which recieves the parsed trace and writes
    a new .json file to the cwd.
    returns a generated json and path to a trace .txt file to callback
  */

  construct_json:
    function (file, callback){
      fs.readFile(file, 'utf8', function (err, data){
        if (err) throw err;
        var lines = data.toString().split("\n")

        // once the json is returned, pass it to the callback which performs I/O
        callback(module.exports.parse(lines), file);
      })
  },


  /* overview: parses portions of the trace .txt file.

    @params: lines: an array of strings obtained by a trace file
    split by the new line char.
    returns a json containing parsed contents of the .txt file
  */

  parse:
    function (lines){

      var total_time = 0 // total time of all jobs within curr trace file
      var trace = {'number_of_jobs':parseInt(lines[0]),
      'simulation_time':parseInt(lines[1]),
      'maximum_length':parseInt(lines[2]),
      }

      // start from the third index and process the jobs information within the file
      trace['jobs'] = module.exports.parse_jobs(lines.splice(3,lines.length-1))
      trace['jobs'].forEach(function(object){
        total_time += parseInt(object['job_length'])
      })
      trace['total_time'] = total_time
      return trace
  },


  /* overview: parses the 'jobs' portions of the trace .txt file.

    @params: lines: an array of strings obtained by a trace file
    split by the new line char and which only contain arrival time and
    job length data.
    returns an array of job jsons
  */

  parse_jobs:
    function (jobs){
      // parses the 'jobs' of the .txt files.
      var job_array = []
      jobs.forEach(function(job){

        var _the_current_job = job.split(' ')
        if(_the_current_job[0] !== ''){ // parse only valid strings

          job_array.push(
            // the time when each CPU burst starts to run
            {'start_time':parseInt(_the_current_job[0]),

            // aka burst time
            // burst time: the amount of time the process uses the processor before it is no longer ready
            'job_length':parseInt(_the_current_job[1])
          })
        }
      })
      return job_array;
  },


  /* overview: passes the generated, parsed data which sits in json to
    algorithms.py by spawning a new process. the function hands off json as
    an argument parameter of the python script. the python script consumes
    the json, computes, prints to file an AWT and awaits other jsons.

    @params: trace: the generated json
    @params: filename: the full path of the original trace file which
    has just been parsed.
    returns void
  */

  pass:
    function (trace, filename){

      console.log(trace);

      // spawn a new process, executes the python binary
      // instructs python to run the algorithms.py script for
      // each parsed json.
      if(trace !== undefined && filename !== undefined){
        const execFile = require('child_process').execFile;
        const child_two = execFile('python', ['algorithms.py', JSON.stringify(trace), filename.split("/").pop().split(".").shift()], function (error, stdout, stderr){
          if (error) {
            throw error;
          }
          console.log(stdout);
        });
      }


    }
};


// function call executes the solution script
module.exports.exec(function(files){

  // spawn a new process, executes system call to remove old solution.txt from dir
  const execFile = require('child_process').execFile;
  const child_one = execFile('rm', ['-rf', 'solution.txt'], function (error, stdout, stderr){
    if (error) {
      throw error;
    }
    console.log(stdout);
  });

  // for each .txt in the dir, parse it, pass it.
  files.forEach(function(filename){
    module.exports.construct_json(curr_folder+filename, module.exports.pass)
  });
});
