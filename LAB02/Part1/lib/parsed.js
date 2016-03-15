var fs = require("fs")
var resources = 'resources/'
var curr_folder = process.cwd().slice(0, -3)  + resources
var trace_objects = []

// package module for external/modular/future use.
module.exports = {
  exec:
    function (callback){
      fs.readdir(curr_folder ,function (err, files){  // locates and returns an array of trace files
        if (err) throw err;

        callback(files)
      })
  },

  construct_json:
    function (file, callback){
      // builds a json  which defines a job data structure from the txt file
      fs.readFile(file, 'utf8', function (err, data){
        if (err) throw err;
        var lines = data.toString().split("\n")

        // once the json is returned, pass it to the callback which performs I/O
        callback(module.exports.parse(lines), file);
      })
  },

  parse:
    function (lines){
      // parses portions of the .txt files.
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

  write:
    function (trace, filename){
      // write json to file
      console.log(trace);
      // names the files according to the trace they came from

      fs.appendFile(filename.split("/").pop().split(".").shift() + '.json', JSON.stringify(trace, null, 2)+"\n\n", function (err){
        if (err) throw err;
        console.log('The "data to append" was appended to file!');
      });
    }
};

// function call executes the parse script
module.exports.exec(function(files){
  // forEach .txt in the dir, parse it, write it.
  files.forEach(function(filename){
    module.exports.construct_json(curr_folder+filename, module.exports.write)
  });
});
