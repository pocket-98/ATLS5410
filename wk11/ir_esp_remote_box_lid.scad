// 3d printable box with lid with holes for buttons
// units all in mm

// base
length = 150;
width = 88;
height = 30;
wall = 3;

// led_hole
led_hole_radius = 3.25;
led_hole_delta_x = 6;
led_hole_delta_z = 12;

// relay hole
relay_hole_width = 6;
relay_hole_height = 3;
relay_hole_delta_x = 32;
relay_hole_delta_z = 6;

// charger hole
charger_hole_width = 8;
charger_hole_height = 3;
charger_hole_delta_x = -10;
charger_hole_delta_z = 6;

// lid
lid_channel_padding = 0.2;
lid_length = 112;

dx = 0.01;
$fn = 128;

module base(length, width, height, wall) {
    union() {
        translate([-length/2,-width/2,0])
            cube([length, width, wall]);
        translate([-length/2,-width/2,wall-dx])
            cube([length, wall, height-wall+dx]);
        translate([-length/2,width/2-wall,wall-dx])
            cube([length, wall, height-wall+dx]);
        translate([-length/2,-width/2+wall-dx,wall-dx])
            cube([wall, width-2*wall+2*dx, height-2*wall+dx]);
        translate([length/2-wall,-width/2+wall-dx,wall-dx])
            cube([wall, width-2*wall+2*dx, height-wall+dx]);
    }
}

module led_hole(box_length, box_wall, hole_radius, delta_x, delta_z) {
    translate([box_length/2-box_wall-dx, -delta_x, delta_z])
        rotate([0, 90, 0])
            cylinder(box_wall+2*dx, hole_radius, hole_radius);
}
module relay_hole(box_length, box_wall, hole_width, hole_height, delta_x, delta_z) {
    translate([-box_length/2-dx, -delta_x, delta_z])
        cube([box_wall+2*dx, hole_width, hole_height]);
}
module charger_hole(box_length, box_wall, hole_width, hole_height, delta_x, delta_z) {
    translate([-box_length/2-dx, -delta_x, delta_z])
        cube([box_wall+2*dx, hole_width, hole_height]);
}

module lid(box_length, box_width, wall, lid_length, lid_padding) {
    union() {
        translate([-box_length/2, -box_width/2, 0])
            cube([lid_length, box_width, wall]);
        translate([-box_length/2, -box_width/2+wall+lid_padding, wall-dx])
            cube([lid_length, box_width-2*wall-2*lid_padding, wall+dx]);
    }
}

difference() {
    base(length, width, height, wall);
    led_hole(length, wall, led_hole_radius, led_hole_delta_x, led_hole_delta_z);
    relay_hole(length, wall, relay_hole_width, relay_hole_height, relay_hole_delta_x, relay_hole_delta_z);
    charger_hole(length, wall, charger_hole_width, charger_hole_height, charger_hole_delta_x, charger_hole_delta_z);
}

//!lid(length, width, wall, lid_length, lid_channel_padding);

translate([0,0,height+wall+2])
    rotate([180,0,0])
        lid(length, width, wall, lid_length, lid_channel_padding);