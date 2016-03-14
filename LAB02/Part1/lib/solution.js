
var fs = require("fs")
var resources = 'resources/'
var curr_folder = process.cwd().slice(0, -3)  + resources
console.log(curr_folder);
var trace_objects = []

var run = function(callback){
  fs.readdir(curr_folder ,function (err, files){  // locates and returns an array of trace files
    if (err) throw err;

    callback(files)
   })
}

run(function(files){
  // forEach txt file in the dir, read it, process it and run the scheduling algorithms on it.
  files.forEach(function(filename){
    simulate(curr_folder+filename, rr_algorithm)
  })
})

// takes a file, and runs the scheduling simulation on it
var simulate = function(file, callback){
  fs.readFile(file, 'utf8', function (err, data){
    if (err) throw err;
    var lines = data.toString().split("\n")

    // once the newly parsed trace file object is returned, pass it to the callback
    // which is a scheduling function
    // Added a reference to the filename in callback
    callback(parse(lines), file);
  })
}

// helper functions which parse the 'jobs' as well as other portions of the .txt files.
function parse(lines){
  var total_time = 0 // total time of all jobs within curr trace file
  var trace = {'number_of_jobs':parseInt(lines[0]),
  'simulation_time':parseInt(lines[1]),
  'maximum_length':parseInt(lines[2]),
  }

  // start from the third index and process the jobs information within the file
  trace['jobs'] = parse_jobs(lines.splice(3,lines.length-1))
  trace['jobs'].forEach(function(object){
    total_time += parseInt(object['job_length'])
  })
  trace['total_time'] = total_time
  return trace
}

function parse_jobs(jobs){
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
}

// file processing has completed.
// begin scheduling logic below.
var rr_algorithm = function(trace, filename){
  console.log(trace);
  // names the files according to the trace they came from

  fs.appendFile(filename.split("/").pop().split(".").shift() + '.json', JSON.stringify(trace, null, 2)+"\n\n", function (err){
    if (err) throw err;
    console.log('The "data to append" was appended to file!');
  });
}


//
//   // for each job, do the following;
//   var process = function(trace, callback){
//     var jobs = trace['jobs']
//
//     // while (true) {
//       for (var i = 0; i < trace['simulation_time']; i++) {
//         var current_process// = jobs[i];
//         console.log(jobs[i]);
//         setTimeout(function() { /*do nothing*/ }, 1000);
//
//
//         if(i == jobs[i]['start_time']){ // take the job off wait queue
//           // put the job on CPU
//           if(jobs[i]['job_length'] == 0 ){ jobs.shift(); } // the job is finished, remove from queue
//           current_process = jobs.shift();
//           console.log("jobs.shift, ", current_process );
//         }
//
//         // compute the new job length after time_slice
//         current_process['job_length']-= 1;
//         jobs.push(current_process);
//
//       }
//     // }
//     callback()
//   }
//
//   process(trace, function(){console.log("\ndone!!\n");})
// }
