
window.onload = function()
{
    var canvas_width = 600;
    var canvas_height;
    var x_res, y_res;
    var unit;
    var color;

    function draw(x, y, weight)
    {
        var canvas = document.getElementById('canvas');

        if (canvas.getContext)
        {
            var ctx = canvas.getContext('2d');
            color = Math.floor(255 * (1 - Math.abs(weight)));
            console.log(color);
            ctx.fillStyle = "rgb(" + color + "," + color + "," + color + ")";
            ctx.fillRect(x, y, unit, unit);
        }
    }

    function readSingleFile(evt) {
        
        var file = evt.target.files[0]; 

        if (file) 
        {
            var read = new FileReader();
            read.onload = function(e) 
            { 
                var cppn = JSON.parse(e.target.result);
               
                if(cppn.dimension != 2)
                {
                    x_res = cppn.resolution.x;
                    y_res = cppn.resolution.y;

                    unit = canvas_width / x_res;
                    canvas_height = unit * y_res;

                    canvas.width = canvas_width;
                    canvas.height = canvas_height;

                    for (var y = 0; y < y_res; y++) 
                        for (var x = 0; x < x_res; x++) 
                            draw(unit*x, unit*y, cppn.values[x][y]);
                }
                else
                {
                    alert("Only 2D CPPNs are drawable under this visualator.");
                } 
            }
            read.readAsText(file);
        }
        else 
        { 
            alert("Failed to load file");
        }
    }
    
    document.getElementById('fileinput').addEventListener('change', readSingleFile, false);
}