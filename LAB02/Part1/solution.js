
var fs = require("fs")
var resources = '/resources/'
var curr_folder = process.cwd() + resources
var trace_objects = {}

fs.readdir(curr_folder ,function (err, files){  // locates and returns an array of trace files
  if (err) throw err;

  files.forEach(function(file){ // forEach txt file in the dir, read it.
    read_trace_file(curr_folder+file)
  })
 })

var read_trace_file = function(file){
  fs.readFile(file, 'utf8', function (err, data){
    if (err) throw err;
    var lines = data.toString().split("\n")

    var trace = {'number_of_jobs':lines[0],
    'simulation_time':lines[1],
    'maximum_lenghth':lines[2],
    }

    trace['jobs'] = parse_jobs(lines.splice(3,lines.length-1)) // start from the third index and process the jobs information within the file
    console.log(trace);
  })
}

// helper function which parses the 'jobs' portions of the .txt files.
function parse_jobs(jobs){
  var job_array = []
  jobs.forEach(function(job){
    var _the_current_job = job.split(' ')

    if(_the_current_job[0] !== '') // parse only valid strings
    job_array.push({'start_time':_the_current_job[0],'job_length':_the_current_job[1]})
  })
  return job_array;
}

// file processing has completed.
// begin scheduling logic below.
