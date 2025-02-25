// 5 rectangles to form a box enclosure with finger joinery

width = 38; // mm
length = 58; // mm
height = 15; // mm

wall = 1; // mm
fingers = 5; // mm
finger_dx = 0.2; //mm


function cut_pts_wf1(l,w,f,o) = floor((l-(o?2*w:0)) / f);
function cut_pts_wf(l,w,f,o) = (cut_pts_wf1(l,w,f,o) % 2 == 1) ?
            ((cut_pts_wf1(l,w,f,o) - 1) / 2) :
            (cut_pts_wf1(l,w,f,o) / 2 - 1);
function cut_pts_p(l,w,f,o) = (cut_pts_wf1(l,w,f,o) % 2 == 1) ?
            ((l - (o?2*w:0) - cut_pts_wf1(l,w,f,o) * f) / 2) :
            ((l - (o?2*w:0) - (cut_pts_wf1(l,w,f,o) - 1) * f) / 2);
function cut_pts(l, w, f, o) =
    concat(
        (cut_pts_p(l,w,f,o) < f / 2) ? [] : [[(o?w:0), (o?w:0)+cut_pts_p(l,w,f,o)]],
        [ for (x=[0:cut_pts_wf(l,w,f,o)-1])
            [(o?w:0)+cut_pts_p(l,w,f,o)+f+2*x*f,
            (o?w:0)+cut_pts_p(l,w,f,o)+2*f+2*x*f]
        ],
        (cut_pts_p(l,w,f,o) < f / 2) ? [] : [[l - (o?w:0) - cut_pts_p(l,w,f,o), l - (o?w:0)]]
    );

module base(
        width = 40,
        length = 60,
        height = 20,
        wall = 2,
        fingers = 5,
        finger_dx = 0.5,
        dx = 0.002,
        dx2 = 0.001
    ) {
    width_cut_pts = cut_pts(width, wall, fingers, 0);
    length_cut_pts = cut_pts(length, wall, fingers, 1);

    // base
    difference() {
        cube([length, width, wall]);
        for (cut=width_cut_pts) {
            translate([0-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }

        for (cut=width_cut_pts) {
            translate([length-wall-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }

        for (cut=length_cut_pts) {
            translate([cut[0]-finger_dx-dx2, 0-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }

        for (cut=length_cut_pts) {
            translate([cut[0]-finger_dx-dx2, width-wall-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }
    }
}


module left(
        width = 40,
        length = 60,
        height = 20,
        wall = 2,
        fingers = 5,
        finger_dx = 0.5,
        dx = 0.002,
        dx2 = 0.001
    ) {
    l2 = concat([0], [for (i=cut_pts(length, wall, fingers,1)) each [i[0],i[1]]], [length]);
    length_cut_pts = [ for (i=[0:2:len(l2)-1]) [l2[i],l2[i+1]] ];
    height_cut_pts = cut_pts(height, wall, fingers, 1);

    // left
    difference() {
        cube([length, height, wall]);

        for (cut=length_cut_pts) {
            translate([cut[0]-finger_dx-dx2, height-wall-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }
        
        for (cut=height_cut_pts) {
            translate([0-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }
        
        for (cut=height_cut_pts) {
            translate([length-wall-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }
    }
}


module right(
        width = 40,
        length = 60,
        height = 20,
        wall = 2,
        fingers = 5,
        finger_dx = 0.5,
        dx = 0.002,
        dx2 = 0.001
    ) {
    l2 = concat([0], [for (i=cut_pts(length, wall, fingers,1)) each [i[0],i[1]]], [length]);
    length_cut_pts = [ for (i=[0:2:len(l2)-1]) [l2[i],l2[i+1]] ];
    height_cut_pts = cut_pts(height, wall, fingers, 1);

    // right
    difference() {
        cube([length, height, wall]);

        for (cut=length_cut_pts) {
            translate([cut[0]-finger_dx-dx2, 0-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }
                
        for (cut=height_cut_pts) {
            translate([0-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }
        
        for (cut=height_cut_pts) {
            translate([length-wall-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }
    }
}
module top(
        width = 40,
        length = 60,
        height = 20,
        wall = 2,
        fingers = 5,
        finger_dx = 0.5,
        dx = 0.002,
        dx2 = 0.001
    ) {
    w2 = concat([0], [for (i=cut_pts(width, wall, fingers,0)) each [i[0],i[1]]], [width]);
    width_cut_pts = [ for (i=[0:2:len(w2)-1]) [w2[i],w2[i+1]] ];
    h2 = concat([0], [for (i=cut_pts(height, wall, fingers,1)) each [i[0],i[1]]], [height]);
    height_cut_pts = [ for (i=[0:2:len(h2)-3]) [h2[i],h2[i+1]] ];


    // top
    translate([height/2, width/2, 0]) rotate([0,0,180])
    translate([-height/2,-width/2,0]) difference() {
        cube([height, width, wall]);

        for (cut=width_cut_pts) {
            translate([0-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }
        
        for (cut=height_cut_pts) {
            translate([cut[0]-finger_dx-dx2, 0-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }
        
        for (cut=height_cut_pts) {
            translate([cut[0]-finger_dx-dx2, width-wall-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }
        
    }
}

module bottom(
        width = 40,
        length = 60,
        height = 20,
        wall = 2,
        fingers = 5,
        finger_dx = 0.5,
        dx = 0.002,
        dx2 = 0.001
    ) {
    w2 = concat([0], [for (i=cut_pts(width, wall, fingers,0)) each [i[0],i[1]]], [width]);
    width_cut_pts = [ for (i=[0:2:len(w2)-1]) [w2[i],w2[i+1]] ];
    h2 = concat([0], [for (i=cut_pts(height, wall, fingers,1)) each [i[0],i[1]]], [height]);
    height_cut_pts = [ for (i=[0:2:len(h2)-3]) [h2[i],h2[i+1]] ];

    // top
    difference() {
        cube([height, width, wall]);

        for (cut=width_cut_pts) {
            translate([0-dx2, cut[0]-finger_dx-dx2, 0-dx2])
                cube([wall+dx, (cut[1]-cut[0])+finger_dx+dx, wall+dx]);
        }
        
        for (cut=height_cut_pts) {
            translate([cut[0]-finger_dx-dx2, 0-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }
        
        for (cut=height_cut_pts) {
            translate([cut[0]-finger_dx-dx2, width-wall-dx2, 0-dx2])
                cube([(cut[1]-cut[0])+finger_dx+dx, wall+dx, wall+dx]);
        }
    }
}

base(
    width = width,
    length = length,
    height = height,
    wall = wall,
    fingers = fingers,
    finger_dx = finger_dx
);

translate([0,-height-1,0]) left(
    width = width,
    length = length,
    height = height,
    wall = wall,
    fingers = fingers,
    finger_dx = finger_dx
);

translate([0,width+1,0]) right(
    width = width,
    length = length,
    height = height,
    wall = wall,
    fingers = fingers,
    finger_dx = finger_dx
);

translate([-height-1,0,0]) top(
    width = width,
    length = length,
    height = height,
    wall = wall,
    fingers = fingers,
    finger_dx = finger_dx
);

translate([length+1,0,0]) bottom(
    width = width,
    length = length,
    height = height,
    wall = wall,
    fingers = fingers,
    finger_dx = finger_dx
);

