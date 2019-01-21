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
        function boundary(T,x)
        {
            return 5*T*(0.2969*Math.sqrt(x)-0.1260*x-0.4531*x*x+0.2843*x*x*x);
        }
        function camber(m,p,x)
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
        tip = translate3d([param.swept*param.width, 0, -param.width],tip);
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
        var wingProfile = generateNacaAirfoil({slice:10, thickness:0.15, maxcamber:0.02, camberpos:0.4});
        var wingAirfoilSection = generateAirfoilSection({points:wingProfile, width:5, tipscale:6/10, swept:0.04});

        var rectSeparator = linear_extrude({height:4}, rotate([0,0,40],square({size: [0.5,0.5], center: true})))
        rectSeparator = rectSeparator.translate([0.9,-0.06,-4]);

        var wingBody = difference(wingAirfoilSection,rectSeparator);
        var teflap = intersection(wingAirfoilSection,rectSeparator);
        teflap = teflap.translate([-0.6,0.1,0]);
        teflap = rotate([0,0,0], teflap); // FLAP ANGLE
        teflap = teflap.translate([0.6,-0.1,0]);
        teflap = color("red",teflap);

        var wingBodyslatmask = cylinder({r1: 0.177, r2: 0.177, h: 5.1, center:true}); 
        wingBodyslatmask = wingBodyslatmask.translate([0.21,-0.090,-2.5]);
        wingBodyslatmask = rotate([0,-Math.atan(0.2/5)*180/Math.PI,0], wingBodyslatmask);
        wingBodyslatmask = color("blue",wingBodyslatmask);

        var slat = difference(wingBody, wingBodyslatmask);
        var slatmask = rotate([0,-Math.atan(0.2/5)*180/Math.PI,0], cube({size: [0.35,0.2,5.1], center: true}))
                .translate([0.11,0,-2.5]);
        slatmask = color("green",slatmask);
        slat = intersection(slat, slatmask);
        slat = color("red",slat);

        wingBody = difference(wingBody, slat);

        slat = slat.translate([-0.15,0,2.5]);
        slat = rotate([0,-0.5+Math.atan(0.2/5)*180/Math.PI,0],slat);
        
        //slat = slat.translate([-0.09,-0.01,0])
        //slat = rotate([0,-1,0],slat);
        //slat = rotate([0,-0.5,30],slat);

        slat = rotate([0,0.5-Math.atan(0.2/5)*180/Math.PI,0],slat);
        slat = slat.translate([0.15,0,-2.5]);

        var wing = union(slat, wingBody)
        wing = union(wing, teflap);
        wing = rotate([0,0,-6], wing);
        wing = rotate([3,0,0], wing);
        wing = wing.translate([-0.5, 0.075,- 0.5]);
        wing = union(wing, mirror([0, 0, 1],wing));
    
        var tailProfile = generateNacaAirfoil({slice:10, thickness:0.1, maxcamber:0, camberpos:0.5});
        var tailAirfoilSection = generateAirfoilSection({points:tailProfile, width: 1, tipscale:6/10, swept:0.3});
        //tailAirfoilSection = scale([(2/3), (2/3), (2/3)],tailAirfoilSection);
        tailAirfoilSection = tailAirfoilSection.translate([0, 0, -0.1]);
        tailAirfoilSection = rotate([-35,0,0], tailAirfoilSection);
        var tailAirfoilSectionZ = mirror([0,0,1], tailAirfoilSection);
        tailAirfoilSection = union(tailAirfoilSection, tailAirfoilSectionZ);
        tailAirfoilSection = tailAirfoilSection.translate([2, 0.25, 0]);

        var fuselageBox = cube({size:[1, 0.5, 1], round:true, center:true}).scale([10,1,1]);
        var fuselageBody = sphere({r:0.475, center: true});
        fuselageBody = fuselageBody.scale([4, 1, 2]);
        fuselageBody = fuselageBody.translate([0,0.1,0]);
        fuselageBody = intersection(fuselageBody, fuselageBox);
        var frame = cube({size:[5, (10/100)*(2/3), (10/100)*(2/3)], center:true});
        frame = frame.translate([0.5, 0.2 ,0]);
        return [wing,
            fuselageBody,
            tailAirfoilSection,
            frame];
    }
    