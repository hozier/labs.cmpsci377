
var fs = require("fs")
var resources = '/resources/'
var curr_folder = process.cwd() + resources
var trace_objects = []

fs.readdir(curr_folder ,function (err, files){  // locates and returns an array of trace files
  if (err) throw err;

  // forEach txt file in the dir, read it, process it and run the scheduling algorithms on it.
  files.forEach(function(filename){
    simulate(curr_folder+filename, rr_algorithm)
  })
 })

// takes a file, a runs the scheduling simulation on it
var simulate = function(file, callback){
  fs.readFile(file, 'utf8', function (err, data){
    if (err) throw err;
    var lines = data.toString().split("\n")

    // once the newly parsed trace file object is returned, pass it to the callback
    // which is a scheduling function
    callback(parse(lines));
  })
}

// helper functions which parse the 'jobs' as well as other portions of the .txt files.
function parse(lines){
  var total_time = 0 // total time of all jobs within curr trace file
  var trace = {'number_of_jobs':lines[0],
  'simulation_time':lines[1],
  'maximum_length':lines[2],
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

    if(_the_current_job[0] !== '') // parse only valid strings
    // burst time: the amount of time the process uses the processor before it is no longer ready
    job_array.push({'start_time':_the_current_job[0],'job_length' /* aka burst time*/ :_the_current_job[1] })
  })
  return job_array;
}

// file processing has completed.
// begin scheduling logic below.
var rr_algorithm = function(trace){
  console.log("trace, ", trace)

}
