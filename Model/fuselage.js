function translate3d(point, points)
    {
        var rv = [];
        for (var i=0; i<points.length; i++)
        {
            rv.push([points[i][0] + point[0], points[i][1] + point[1], points[i][2] + point[2]]);
        }
        return rv;
    }

    function scale3d(point, points)
    {
        var rv = [];
        for (var i=0; i<points.length; i++)
        {
            rv.push([points[i][0]*point[0], points[i][1]*point[1], points[i][2]*point[2]]);
        }
        return rv;
    }

    function generateNacaAirfoil(param)
    {
        function boundary(T, x)
        {
            return 5*T*(0.2969*Math.sqrt(x)-0.1260*x-0.4531*x*x+0.2843*x*x*x);
        }
        function camber(m, p, x)
        {
            if (0<=x && x<=p)
                return (m*(2*p*x-x*x))/(p*p);
            return (m*(1-2*p+2*p*x-x*x))/((1-p)*(1-p));
        }

        var top = [];
        var bot = [];
        for (var i = 0; i<=param.slice; i++)
        {
            var x = i/param.slice;
            top.push([x, boundary(param.thickness, x)+camber(param.maxcamber, param.camberpos, x),0]);
            bot.push([x, camber(param.maxcamber, param.camberpos, x) - boundary(param.thickness, x),0]);
        }

        return top.concat(bot);
    }

    function generateAirfoilSection(param)
    {
        function generateLeadingFaces(points, nslices)
        {
            var a=0;
            var b=nslices;
            var c=2*nslices;
            var d=3*nslices;
            return [
                [a, a+1, b+1], // base face
                [c, d+1, c+1], // tip face
                [a, c, c+1], // top cover
                [a, c+1, a+1],
                [a, d+1, c], // bottom cover
                [a, b+1, d+1]];
        }

        function generateBodyFaces(points, nslices, n)
        {
            var a=n;
            var b=nslices + n;
            var c=2*nslices + n;
            var d=3*nslices + n;
            return [
                [a, a+1, b], // base face
                [b, a+1, b+1],
                [c, d, c+1],// tip face
                [d, d+1, c+1],
                [a, c, c+1], // top cover
                [a, c+1, a+1],
                [b, d+1, d], // bottom cover
                [b, b+1, d+1]];
        }
        function generateTrailingFaces(points, nslices)
        {
            var n = nslices-2;
            var a=n;
            var b=nslices + n;
            var c=2*nslices + n;
            var d=3*nslices + n;
            return [
                [a, b+1, b], // base face
                [c, d, d+1], // tip face
                [a, c, d+1], // top cover
                [a, d+1, b+1],
                [b, d+1, d], // bottom cover
                [b, b+1, d+1]];
        }

        var tip = scale3d([param.tipscale, param.tipscale, 0], param.points);
        tip = translate3d([param.swept*param.length, 0, -param.length], tip);
        var points = param.points.concat(tip);
        var nslices = points.length/4;

        var faces  = [];
        faces = faces.concat(generateLeadingFaces(points, nslices));
        for (var i=1; i<(nslices-2);i++)
        {
            var t = generateBodyFaces(points, nslices, i);
            faces = faces.concat(t);
        }

        faces = faces.concat(generateTrailingFaces(points, nslices));

        return polyhedron({points:points, triangles: faces});
    }

    function main()
    {
        var FLAP_ANGLE = -0;
        var SLAT_ANGLE = 0;
        var COVER_SLAT = true;
        var COVER_BODY = true;
        var COVER_FLAP = false;
        var AIRFOIL_RESOLUTION = 150;
        var IS_LEFT = false;
        var CHORD_LEN = 1.5;
        var TIP_SCALE = 6/10;
        var WING_LEN = 5;
        var FLAP_LEN = 3.5;
        var RIB_THICKNESS = (0.2/10);

        var rv = [];

        var profile = generateNacaAirfoil({slice:AIRFOIL_RESOLUTION, thickness:0.15, maxcamber:0.02, camberpos:0.4});
        var airfoilSection = generateAirfoilSection({points:profile, tipscale: TIP_SCALE,  length: WING_LEN*(1/CHORD_LEN), swept:0.04});

        airfoilSection = airfoilSection.scale([CHORD_LEN, CHORD_LEN, CHORD_LEN]);

        // FLAP MASK
        var rectSeparator = linear_extrude({height:FLAP_LEN+RIB_THICKNESS}, rotate([0,0,40], square({size: [0.5,0.5], center: true})));
        rectSeparator = rectSeparator.scale([CHORD_LEN, CHORD_LEN, 1]);
        rectSeparator = rectSeparator.translate([(CHORD_LEN)*0.9, -(CHORD_LEN)*0.06, -(FLAP_LEN+RIB_THICKNESS)]);


        // WING BODY AND FLAP
        var wingBody = difference(airfoilSection, rectSeparator);
        var teflap = intersection(airfoilSection, rectSeparator);

        // SLAT MASK
        var wingBodyslatmask = cylinder({r1: (CHORD_LEN*TIP_SCALE*0.239), r2: (CHORD_LEN*0.177), h: WING_LEN+0.1, center:true});
        wingBodyslatmask = wingBodyslatmask.translate([CHORD_LEN*0.21, -CHORD_LEN*0.090, -WING_LEN/2]);
        wingBodyslatmask = rotate([0,-Math.atan(0.2/5)*180/Math.PI,0], wingBodyslatmask);
        wingBodyslatmask = color("blue", wingBodyslatmask);

        var slat = difference(wingBody, wingBodyslatmask);
        var slatmask = rotate([0,-Math.atan(0.2/5)*180/Math.PI,0], cube({size: [0.3,0.2, WING_LEN+0.1], center: true}));
        slatmask = slatmask.scale([CHORD_LEN, CHORD_LEN, 1]);
        slatmask = slatmask.translate([CHORD_LEN*0.11, 0, -WING_LEN/2]);
        slatmask = color("green", slatmask);
        slat = intersection(slat, slatmask);
        slat = color("red", slat);

        wingBody = difference(wingBody, slat);

        slat = slat.translate([-CHORD_LEN*0.15, 0, CHORD_LEN*2.5]);
        slat = rotate([0,-0.5+Math.atan(0.2/5)*180/Math.PI,0], slat);
        slat = rotate([0,0, SLAT_ANGLE], slat);
        slat = slat.translate([-CHORD_LEN*0.15, CHORD_LEN*0.2, 0]); // SLAT CENTER
        slat = rotate([0,0, SLAT_ANGLE*2], slat);
        slat = slat.translate([CHORD_LEN*0.15, -CHORD_LEN*0.2, 0]); // SLAT CENTER
        slat = rotate([0,0.5-Math.atan(0.2/5)*180/Math.PI,0], slat);
        slat = slat.translate([CHORD_LEN*0.15, 0, -CHORD_LEN*2.5]);

        teflap = teflap.translate([-CHORD_LEN*0.6, CHORD_LEN*0.1, 0]);
        teflap = rotate([0,0, FLAP_ANGLE], teflap); // FLAP ANGLE
        teflap = teflap.translate([CHORD_LEN*0.6, -CHORD_LEN*0.1, 0]);
        teflap = color("red", teflap);

        var wing = union(slat, wingBody);
        wing = union(wing, teflap);
        wing = rotate([0,0,-6], wing);
        wing = rotate([3,0,0], wing);
        wing = wing.translate([-0.5, 0.075,-0.5]);
        wing = union(wing, mirror([0, 0, 1],wing));

        var tailProfile = generateNacaAirfoil({slice:AIRFOIL_RESOLUTION, thickness:0.1, maxcamber:0, camberpos:0.5});
        var tailAirfoilSection = generateAirfoilSection({points:tailProfile, length: 1.5, tipscale:TIP_SCALE, swept:0.3});

        tailAirfoilSection = tailAirfoilSection.translate([0, 0, -(0.5/10)]);
        tailAirfoilSection = rotate([35,0,0], tailAirfoilSection);
        var tailAirfoilSectionZ = mirror([0,0,1], tailAirfoilSection);
        tailAirfoilSection = union(tailAirfoilSection, tailAirfoilSectionZ);
        tailAirfoilSection = tailAirfoilSection.translate([2, 0.25, 0]);

        //return [wing, tailAirfoilSection];
        var fuselageBox = cube({size:[1, 1, 1], round:true, center:true}).scale([10,0.75,1]);
        var fuselageBody = sphere({r:0.475, center: true});
        fuselageBody = fuselageBody.scale([4.3, 1, 1.3]);
        fuselageBody = fuselageBody.translate([0,0.1,0]);
        fuselageBody = intersection(fuselageBody, fuselageBox);

        var frame = cube({size:[4, (0.5/10), (0.5/10)], center:true});
        frame = frame.translate([1, 0.2 ,0]);

        var rwSclale = [10/100, 10/100, 10/100];
        return rv
            .concat(wing.scale(rwSclale))
            .concat(fuselageBody.scale(rwSclale))
            .concat(tailAirfoilSection.scale(rwSclale))
            .concat(frame.scale(rwSclale));
    }
