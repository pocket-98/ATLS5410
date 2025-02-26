// circular box 2d outline base and kerf bend laser cut hinge walls
// https://www.instructables.com/Curved-laser-bent-wood/
// https://www.defproc.co.uk/?s=lattice+hinges

radius = 60;
height = 20;

pattern = 1;
kerf_space = 1;

stretch = (pattern == 1) ? 3 : 1;

fn = 64;

function wall_length(radius) = 2*PI*radius;

translate([-radius-1, 0, 0]) circle(radius, $fn=fn);

length = wall_length(radius);


N = floor(length/(kerf_space*1));
M = floor(height/(kerf_space*(stretch+1)));
echo(M);

difference() {
    square([height, length]);
    for (i=[1:N]) {
        if (i % 2 == 0) {
            for (j=[1:M]) {
                translate([j/(M+1)*height, i/(N+1)*length, 0])
                    scale([stretch,1,1])
                        circle(kerf_space/2, $fn=fn);
            }
         } else {
            for (j=[0:M]) {
                translate([(0.5+j)/(M+1)*height, i/(N+1)*length, 0])
                    scale([stretch,1,1])
                        circle(kerf_space/2, $fn=fn);
            }
         }
    }
}