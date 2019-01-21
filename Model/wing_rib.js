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

        var tip = scale3d([6/10, 6/10, 0], param.points);
        tip = translate3d([param.swept*5, 0, -5],tip);
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
        var FOR_PRINTING = false;
        var FLAP_ANGLE = -35;
        var SLAT_ANGLE = 10;
        var COVER_SLAT = false;
        var COVER_BODY = false;
        var COVER_FLAP = false;
        var AIRFOIL_RESOLUTION = 100;
        if (FOR_PRINTING)
        {
            FLAP_ANGLE = 0;
            SLAT_ANGLE = 0;
        }
        
        var profile = generateNacaAirfoil({slice:AIRFOIL_RESOLUTION, thickness:0.15, maxcamber:0.02, camberpos:0.4});
        var airfoilSection = generateAirfoilSection({points:profile, swept:0.04});

        var rectSeparator = linear_extrude({height:4}, rotate([0,0,40],square({size: [0.5,0.5], center: true}))).translate([0.9,-0.06,-4]);


        var wingBody = difference(airfoilSection,rectSeparator);
 
       var teflap = intersection(airfoilSection,rectSeparator);

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

        var ribHolderHole = cube({size:[(5/100)*(2/3),(5/100)*(2/3),5.1], center:true});
        var ribHolder = ribHolderHole.scale([1.5,1.5,1]);
        ribHolder = difference(ribHolder, ribHolderHole);
        ribHolder = ribHolder.translate([0.35,0,-2.5]);
        ribHolder = color("black",ribHolder);
        ribHolderHole = ribHolderHole.translate([0.35,0,-2.5]);

        var slatHolderHole = cylinder({r1: (2.5/100)*(2/3), r2: (2.5/100)*(2/3), h:5.1, center:true});
        var slatHolder = slatHolderHole.scale([1.3, 1.3, 1]);
        slatHolder = difference(slatHolder, slatHolderHole);
        slatHolder = rotate([0,-Math.atan(0.2/5)*180/Math.PI,0], slatHolder);
        slatHolder = slatHolder.translate([0.13, 0, -2.5]);
        slatHolder = color("black",slatHolder);
        slatHolderHole = rotate([0,-Math.atan(0.2/5)*180/Math.PI,0], slatHolderHole);
        slatHolderHole = slatHolderHole.translate([0.13, 0, -2.5]);
        slat = difference(slat, slatHolderHole);

        var teflapHolderHole = cylinder({r1: (2.5/100)*(2/3), r2: (2.5/100)*(2/3), h:4, center:true});
        var teflapHolder = teflapHolderHole.scale([1.1, 1.1, 1]);
        teflapHolder = difference(teflapHolder, teflapHolderHole);
        teflapHolder = teflapHolder.translate([0.63, 0.01, -2]);
        teflapHolder = color("black",teflapHolder);
        teflapHolderHole = teflapHolderHole.translate([0.63, 0.01, -2]);

        wingBody = difference(wingBody, ribHolderHole);
        wingBody = difference(wingBody, ribHolderHole.translate([0.1,0.01,0]));
        teflap = difference(teflap, teflapHolderHole);


        slat = slat.translate([-0.15,0,2.5]);
        slat = rotate([0,-0.5+Math.atan(0.2/5)*180/Math.PI,0],slat);
        slat = rotate([0,0,SLAT_ANGLE],slat);
        slat = slat.translate([-0.15,0.2,0]); // SLAT CENTER
        slat = rotate([0,0,SLAT_ANGLE*2],slat);
        slat = slat.translate([0.15,-0.2,0]); // SLAT CENTER
        slat = rotate([0,0.5-Math.atan(0.2/5)*180/Math.PI,0],slat);
        slat = slat.translate([0.15,0,-2.5]);

        slatHolder = slatHolder.translate([-0.15,0,2.5]);
        slatHolder = rotate([0,-0.5+Math.atan(0.2/5)*180/Math.PI,0],slatHolder);
        slatHolder = rotate([0,0,SLAT_ANGLE],slatHolder);
        slatHolder = slatHolder.translate([-0.15,0.2,0]); // SLAT CENTER
        slatHolder = rotate([0,0,SLAT_ANGLE*2],slatHolder);
        slatHolder = slatHolder.translate([0.15,-0.2,0]); // SLAT CENTER
        slatHolder = rotate([0,0.5-Math.atan(0.2/5)*180/Math.PI,0],slatHolder);
        slatHolder = slatHolder.translate([0.15,0,-2.5]);

        teflap = teflap.translate([-0.6,0.1,0]);
        teflap = rotate([0,0,FLAP_ANGLE], teflap); // FLAP ANGLE
        teflap = teflap.translate([0.6,-0.1,0]);
        teflapHolder = teflapHolder.translate([-0.6,0.1,0]);
        teflapHolder = rotate([0,0,FLAP_ANGLE], teflapHolder); // FLAP ANGLE
        teflapHolder = teflapHolder.translate([0.6,-0.1,0]);
        teflap = color("red",teflap);
        
        var thinSection = cube({size:[2,1,(2/100)*(2/3)], center:true}).translate([0.5,0,-(2/100)*(2/3)/2]);
        var thicksection = cube({size:[2,1,(4/100)*(2/3)], center:true}).translate([0.5,0,-(2/100)*(4/3)/2]);
        
        var slatSections = [];
        var wingBodysections = [];
        var teflapections = [];

        var z = 0;
        for (var i = 0; i<11; i++)
        {
            if (i==10)
            {
                thinSection = thinSection.translate([0,0, (2/100)*(2/3)]);
                thicksection = thicksection.translate([0,0, (2/100)*(2/3)]);
                z -= (2/100)*(2/3);
            }

            var slatSliceHolder = intersection(slatHolder, thicksection);
            var slatSlice = intersection(slat, thinSection);
            slatSlice = union(slatSliceHolder, slatSlice);
            if (FOR_PRINTING)
            {
                slatSlice = slatSlice.translate([-0.025,i/5,z]);
                slatSlice = rotate([180,0,0],slatSlice);
                slatSlice = slatSlice.scale([1.5,1.5,1.5]);
            }
            slatSections.push(slatSlice);
            
            var wingBodysliceHolder = intersection(ribHolder, thicksection);
            var wingBodyslice = intersection(wingBody, thinSection);
            wingBodyslice = union(wingBodysliceHolder, wingBodyslice);

            if (FOR_PRINTING)
            {
                wingBodyslice = wingBodyslice.translate([-0.01,i/5,z]);
                wingBodyslice = rotate([180,0,0],wingBodyslice);
                wingBodyslice = wingBodyslice.scale([1.5,1.5,1.5]);
            }
            wingBodysections.push(wingBodyslice);

            var teflapsliceHolder = intersection(teflapHolder, thicksection);
            var teflapslice = intersection(teflap, thinSection);
            teflapslice = union(teflapsliceHolder, teflapslice);

            if (FOR_PRINTING)
            {
                teflapslice = teflapslice.translate([0.01,i/5,z]);
                teflapslice = rotate([180,0,0],teflapslice);
                teflapslice = teflapslice.scale([1.5,1.5,1.5]);
            }
            teflapections.push(teflapslice);

            thinSection = thinSection.translate([0,0, -0.5]);
            thicksection = thicksection.translate([0,0,-0.5]);
            z += 0.5;
        }

        var rv = []

        if (!FOR_PRINTING)
        {
            if (COVER_SLAT) rv.push(slat);
            if (COVER_BODY) rv.push(wingBody);
            if (COVER_FLAP) rv.push(teflap);
            rv.push(ribHolder);
            rv.push(slatHolder);
            rv.push(teflapHolder);
        }

        return rv
            .concat(slatSections)
            .concat(wingBodysections)
            .concat(teflapections);
    }