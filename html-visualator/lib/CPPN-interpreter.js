
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
            color = Math.floor(255 * (1 - weight) / 2);
            console.log(color);
            ctx.fillStyle = "rgb(" + color + "," + color + "," + color + ")";
            ctx.fillRect(x, y, unit, unit);
        }
    }

    function readSingleFile(evt) {
        //Retrieve the first (and only!) File from the FileList object
        var f = evt.target.files[0]; 

        if (f) 
        {
            var r = new FileReader();
            r.onload = function(e) 
            { 
                var contents = e.target.result;
                var lst = contents.split("\n");
                var weight;
    
                x_res = lst[0];
                y_res = lst[1];

                unit = canvas_width / x_res;
                canvas_height = unit * y_res;

                canvas.width = canvas_width;
                canvas.height = canvas_height;

                for (var y = 0; y < y_res; y++) {

                    var weights = lst[y+2].split("\t");

                    for (var x = 0; x < x_res; x++) draw(unit*x, unit*y, weights[x]); 
                }
            }
            r.readAsText(f);
        }
        else 
        { 
            alert("Failed to load file");
        }
    }
    
    document.getElementById('fileinput').addEventListener('change', readSingleFile, false);
}