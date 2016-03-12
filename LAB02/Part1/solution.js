
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

    var job_array = []
    var jobs = lines.splice(3,lines.length-1) // start from the third index and process the jobs information within the file
    jobs.forEach(function(job){
      var _the_current_job = job.split(' ')
      job_array.push({'start_time':_the_current_job[0],'job_length':_the_current_job[1]})
    })

    trace['jobs'] = job_array
    console.log(trace);
  })
}
