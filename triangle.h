/*

		http://www-2.cs.cmu.edu/~quake/triangle.html

Triangle
A Two-Dimensional Quality Mesh Generator and Delaunay Triangulator.
Version 1.4

Copyright 1993, 1995, 1997, 1998, 2002 Jonathan Richard Shewchuk
2360 Woolsey #H / Berkeley, California 94705-1927
Bugs/comments to jrs@cs.berkeley.edu
Created as part of the Archimedes project (tools for parallel FEM).
Supported in part by NSF Grant CMS-9318163 and an NSERC 1967 Scholarship.
There is no warranty whatsoever.  Use at your own risk.
This executable is compiled for double precision arithmetic.


Triangle generates exact Delaunay triangulations, constrained Delaunay
triangulations, Voronoi diagrams, and quality conforming Delaunay
triangulations.  The latter can be generated with no small angles, and are
thus suitable for finite element analysis.  If no command line switches are
specified, your .node input file is read, and the Delaunay triangulation is
returned in .node and .ele output files.  The command syntax is:

triangle [-prq__a__uAcjevngBPNEIOXzo_YS__LiFlsCQVh] input_file

Underscores indicate that numbers may optionally follow certain switches.
Do not leave any space between a switch and its numeric parameter.
input_file must be a file with extension .node, or extension .poly if the
-p switch is used.  If -r is used, you must supply .node and .ele files,
and possibly a .poly file and an .area file as well.  The formats of these
files are described below.

Command Line Switches:

    -p  Reads a Planar Straight Line Graph (.poly file), which can specify
        vertices, segments, holes, regional attributes, and area
        constraints.  Generates a constrained Delaunay triangulation (CDT)
        fitting the input; or, if -s, -q, -a, or -u is used, a conforming
        constrained Delaunay triangulation (CCDT).  If -p is not used,
        Triangle reads a .node file by default.

    -r  Refines a previously generated mesh.  The mesh is read from a .node
        file and an .ele file.  If -p is also used, a .poly file is read
        and used to constrain segments in the mesh.  If -a is also used
        (with no number following), an .area file is read and used to
        impose area constraints on the mesh.  Further details on refinement
        are given below.

    -q  Quality mesh generation by my variant of Jim Ruppert's Delaunay
        refinement algorithm.  Adds vertices to the mesh to ensure that no
        angles smaller than 20 degrees occur.  An alternative minimum angle
        may be specified after the `q'.  If the minimum angle is 20.7
        degrees or smaller, the triangulation algorithm is mathematically
        guaranteed to terminate (assuming infinite precision arithmetic--
        Triangle may fail to terminate if you run out of precision).  In
        practice, the algorithm often succeeds for minimum angles up to
        33.8 degrees.  For some meshes, however, it may be necessary to
        reduce the minimum angle to avoid problems associated with
        insufficient floating-point precision.  The specified angle may
        include a decimal point.

    -a  Imposes a maximum triangle area.  If a number follows the `a', no
        triangle is generated whose area is larger than that number.  If no
        number is specified, an .area file (if -r is used) or .poly file
        (if -r is not used) specifies a set of maximum area constraints.
        An .area file contains a separate area constraint for each
        triangle, and is useful for refining a finite element mesh based on
        a posteriori error estimates.  A .poly file can optionally contain
        an area constraint for each segment-bounded region, thereby
        controlling triangle densities in a first triangulation of a PSLG.
        You can impose both a fixed area constraint and a varying area
        constraint by invoking the -a switch twice, once with and once
        without a number following.  Each area specified may include a
        decimal point.

    -u  Imposes a user-defined constraint on triangle size.  There are two
        ways to use this feature.  One is to edit the triunsuitable()
        procedure in triangle.c to encode any constraint you like, then
        recompile Triangle.  The other is to compile triangle.c with the
        EXTERNAL_TEST symbol set (compiler switch -DEXTERNAL_TEST), then
        link Triangle against a separate object file that implements
        triunsuitable().  In either case, the -u switch causes the user-
        defined test to be applied to every triangle.

    -A  Assigns an additional attribute to each triangle that identifies
        what segment-bounded region each triangle belongs to.  Attributes
        are assigned to regions by the .poly file.  If a region is not
        explicitly marked by the .poly file, triangles in that region are
        assigned an attribute of zero.  The -A switch has an effect only
        when the -p switch is used and the -r switch is not.

    -c  Creates segments on the convex hull of the triangulation.  If you
        are triangulating a vertex set, this switch causes a .poly file to
        be written, containing all edges in the convex hull.  If you are
        triangulating a PSLG, this switch specifies that the whole convex
        hull of the PSLG should be triangulated, regardless of what
        segments the PSLG has.  If you do not use this switch when
        triangulating a PSLG, it is assumed that you have identified the
        region to be triangulated by surrounding it with segments of the
        input PSLG.  Beware:  if you are not careful, this switch can cause
        the introduction of an extremely thin angle between a PSLG segment
        and a convex hull segment, which can cause overrefinement (and
        possibly failure if Triangle runs out of precision).  If you are
        refining a mesh, the -c switch works differently; it generates the
        set of boundary edges of the mesh (useful if no .poly file was
        read).

    -j  Jettisons vertices that are not part of the final triangulation
        from the output .node file.  By default, Triangle copies all
        vertices in the input .node file to the output .node file, in the
        same order, so their indices do not change.  The -j switch prevents
        duplicated input vertices from appearing in the output .node file;
        hence, if two input vertices have exactly the same coordinates,
        only the first appears in the output.  If any vertices are
        jettisoned, the vertex numbering in the output .node file differs
        from that of the input .node file.

    -e  Outputs (to an .edge file) a list of edges of the triangulation.

    -v  Outputs the Voronoi diagram associated with the triangulation.
        Does not attempt to detect degeneracies, so some Voronoi vertices
        may be duplicated.  See the discussion of Voronoi diagrams below.

    -n  Outputs (to a .neigh file) a list of triangles neighboring each
        triangle.

    -g  Outputs the mesh to an Object File Format (.off) file, suitable for
        viewing with the Geometry Center's Geomview package.

    -B  No boundary markers in the output .node, .poly, and .edge output
        files.  See the detailed discussion of boundary markers below.

    -P  No output .poly file.  Saves disk space, but you lose the ability
        to maintain constraining segments on later refinements of the mesh.

    -N  No output .node file.

    -E  No output .ele file.

    -I  No iteration numbers.  Suppresses the output of .node and .poly
        files, so your input files won't be overwritten.  (If your input is
        a .poly file only, a .node file is written.)  Cannot be used with
        the -r switch, because that would overwrite your input .ele file.
        Shouldn't be used with the -q, -a, -u, or -s switch if you are
        using a .node file for input, because no .node file is written, so
        there is no record of any added Steiner points.

    -O  No holes.  Ignores the holes in the .poly file.

    -X  No exact arithmetic.  Normally, Triangle uses exact floating-point
        arithmetic for certain tests if it thinks the inexact tests are not
        accurate enough.  Exact arithmetic ensures the robustness of the
        triangulation algorithms, despite floating-point roundoff error.
        Disabling exact arithmetic with the -X switch causes a small
        improvement in speed and creates the possibility (albeit small)
        that Triangle will fail to produce a valid mesh.  Not recommended.

    -z  Numbers all items starting from zero (rather than one).  Note that
        this switch is normally overrided by the value used to number the
        first vertex of the input .node or .poly file.  However, this
        switch is useful when calling Triangle from another program.

    -o2 Generates second-order subparametric elements with six nodes each.

    -Y  No new vertices on the boundary.  This switch is useful when the
        mesh boundary must be preserved so that it conforms to some
        adjacent mesh.  Be forewarned that you will probably sacrifice some
        of the quality of the mesh; Triangle will try, but the resulting
        mesh may contain triangles of poor aspect ratio.  Works well if all
        the boundary vertices are closely spaced.  Specify this switch
        twice (`-YY') to prevent all segment splitting, including internal
        boundaries.

    -S  Specifies the maximum number of Steiner points (vertices that are
        not in the input, but are added to meet the constraints on minimum
        angle and maximum area).  The default is to allow an unlimited
        number.  If you specify this switch with no number after it,
        the limit is set to zero.  Triangle always adds vertices at segment
        intersections, even if it needs to use more vertices than the limit
        you set.  When Triangle inserts segments by splitting (-s), it
        always adds enough vertices to ensure that all the segments of the
        PLSG are recovered, ignoring the limit if necessary.

    -L  Do not use diametral lenses to determine whether subsegments are
        encroached; use diametral circles instead (as in Ruppert's
        algorithm).  Use this switch if you want all triangles in the mesh
        to be Delaunay, and not just constrained Delaunay; or if you want
        to ensure that all Voronoi vertices lie within the triangulation.
        (Applications such as some finite volume methods may have this
        requirement.)  This switch may increase the number of vertices in
        the mesh to meet these constraints.

    -i  Uses an incremental rather than divide-and-conquer algorithm to
        form a Delaunay triangulation.  Try it if the divide-and-conquer
        algorithm fails.

    -F  Uses Steven Fortune's sweepline algorithm to form a Delaunay
        triangulation.  Warning:  does not use exact arithmetic for all
        calculations.  An exact result is not guaranteed.

    -l  Uses only vertical cuts in the divide-and-conquer algorithm.  By
        default, Triangle uses alternating vertical and horizontal cuts,
        which usually improve the speed except with vertex sets that are
        small or short and wide.  This switch is primarily of theoretical
        interest.

    -s  Specifies that segments should be forced into the triangulation by
        recursively splitting them at their midpoints, rather than by
        generating a constrained Delaunay triangulation.  Segment splitting
        is true to Ruppert's original algorithm, but can create needlessly
        small triangles.  This switch is primarily of theoretical interest.

    -C  Check the consistency of the final mesh.  Uses exact arithmetic for
        checking, even if the -X switch is used.  Useful if you suspect
        Triangle is buggy.

    -Q  Quiet:  Suppresses all explanation of what Triangle is doing,
        unless an error occurs.

    -V  Verbose:  Gives detailed information about what Triangle is doing.
        Add more `V's for increasing amount of detail.  `-V' gives
        information on algorithmic progress and more detailed statistics.
        `-VV' gives vertex-by-vertex details, and prints so much that
        Triangle runs much more slowly.  `-VVVV' gives information only
        a debugger could love.

    -h  Help:  Displays these instructions.

Definitions:

  A Delaunay triangulation of a vertex set is a triangulation whose
  vertices are the vertex set, wherein no vertex in the vertex set falls in
  the interior of the circumcircle (circle that passes through all three
  vertices) of any triangle in the triangulation.

  A Voronoi diagram of a vertex set is a subdivision of the plane into
  polygonal regions (some of which may be infinite), where each region is
  the set of points in the plane that are closer to some input vertex than
  to any other input vertex.  (The Voronoi diagram is the geometric dual of
  the Delaunay triangulation.)

  A Planar Straight Line Graph (PSLG) is a set of vertices and segments.
  Segments are simply edges, whose endpoints are vertices in the PSLG.
  Segments may intersect each other only at their endpoints.  The file
  format for PSLGs (.poly files) is described below.

  A constrained Delaunay triangulation (CDT) of a PSLG is similar to a
  Delaunay triangulation, but each PSLG segment is present as a single edge
  in the triangulation.  (A constrained Delaunay triangulation is not truly
  a Delaunay triangulation.)  By definition, a CDT does not have any
  vertices other than those specified in the input PSLG.

  A conforming Delaunay triangulation of a PSLG is a true Delaunay
  triangulation in which each PSLG segment is represented by a linear
  contiguous sequence of edges in the triangulation.  Each input segment
  may have been subdivided into shorter subsegments by the insertion of
  additional vertices.  These inserted vertices are necessary to maintain
  the Delaunay property while ensuring that every segment is represented.

File Formats:

  All files may contain comments prefixed by the character '#'.  Vertices,
  triangles, edges, holes, and maximum area constraints must be numbered
  consecutively, starting from either 1 or 0.  Whichever you choose, all
  input files must be consistent; if the vertices are numbered from 1, so
  must be all other objects.  Triangle automatically detects your choice
  while reading the .node (or .poly) file.  (When calling Triangle from
  another program, use the -z switch if you wish to number objects from
  zero.)  Examples of these file formats are given below.

  .node files:
    First line:  <# of vertices> <dimension (must be 2)> <# of attributes>
                                           <# of boundary markers (0 or 1)>
    Remaining lines:  <vertex #> <x> <y> [attributes] [boundary marker]

    The attributes, which are typically floating-point values of physical
    quantities (such as mass or conductivity) associated with the nodes of
    a finite element mesh, are copied unchanged to the output mesh.  If -q,
    -a, -u, or -s is selected, each new Steiner point added to the mesh
    has attributes assigned to it by linear interpolation.

    If the fourth entry of the first line is `1', the last column of the
    remainder of the file is assumed to contain boundary markers.  Boundary
    markers are used to identify boundary vertices and vertices resting on
    PSLG segments; a complete description appears in a section below.  The
    .node file produced by Triangle contains boundary markers in the last
    column unless they are suppressed by the -B switch.

  .ele files:
    First line:  <# of triangles> <nodes per triangle> <# of attributes>
    Remaining lines:  <triangle #> <node> <node> <node> ... [attributes]

    Nodes are indices into the corresponding .node file.  The first three
    nodes are the corner vertices, and are listed in counterclockwise order
    around each triangle.  (The remaining nodes, if any, depend on the type
    of finite element used.)

    The attributes are just like those of .node files.  Because there is no
    simple mapping from input to output triangles, an attempt is made to
    interpolate attributes, which may result in a good deal of diffusion of
    attributes among nearby triangles as the triangulation is refined.
    Attributes do not diffuse across segments, so attributes used to
    identify segment-bounded regions remain intact.

    In .ele files produced by Triangle, each triangular element has three
    nodes (vertices) unless the -o2 switch is used, in which case
    subparametric quadratic elements with six nodes each are generated.
    The first three nodes are the corners in counterclockwise order, and
    the fourth, fifth, and sixth nodes lie on the midpoints of the edges
    opposite the first, second, and third vertices, respectively.

  .poly files:
    First line:  <# of vertices> <dimension (must be 2)> <# of attributes>
                                           <# of boundary markers (0 or 1)>
    Following lines:  <vertex #> <x> <y> [attributes] [boundary marker]
    One line:  <# of segments> <# of boundary markers (0 or 1)>
    Following lines:  <segment #> <endpoint> <endpoint> [boundary marker]
    One line:  <# of holes>
    Following lines:  <hole #> <x> <y>
    Optional line:  <# of regional attributes and/or area constraints>
    Optional following lines:  <region #> <x> <y> <attribute> <max area>

    A .poly file represents a PSLG, as well as some additional information.
    The first section lists all the vertices, and is identical to the
    format of .node files.  <# of vertices> may be set to zero to indicate
    that the vertices are listed in a separate .node file; .poly files
    produced by Triangle always have this format.  A vertex set represented
    this way has the advantage that it may easily be triangulated with or
    without segments (depending on whether the .poly or .node file is
    read).

    The second section lists the segments.  Segments are edges whose
    presence in the triangulation is enforced (although each segment may be
    subdivided into smaller edges).  Each segment is specified by listing
    the indices of its two endpoints.  This means that you must include its
    endpoints in the vertex list.  Each segment, like each point, may have
    a boundary marker.

    If -q, -a, -u, and -s are not selected, Triangle produces a constrained
    Delaunay triangulation (CDT), in which each segment appears as a single
    edge in the triangulation.  If -q, -a, -u, or -s is selected, Triangle
    produces a conforming constrained Delaunay triangulation (CCDT), in
    which segments may be subdivided into smaller edges.  If -L is selected
    as well, Triangle produces a conforming Delaunay triangulation, so
    every triangle is Delaunay, and not just constrained Delaunay.

    The third section lists holes (and concavities, if -c is selected) in
    the triangulation.  Holes are specified by identifying a point inside
    each hole.  After the triangulation is formed, Triangle creates holes
    by eating triangles, spreading out from each hole point until its
    progress is blocked by PSLG segments; you must be careful to enclose
    each hole in segments, or your whole triangulation might be eaten away.
    If the two triangles abutting a segment are eaten, the segment itself
    is also eaten.  Do not place a hole directly on a segment; if you do,
    Triangle chooses one side of the segment arbitrarily.

    The optional fourth section lists regional attributes (to be assigned
    to all triangles in a region) and regional constraints on the maximum
    triangle area.  Triangle reads this section only if the -A switch is
    used or the -a switch is used without a number following it, and the -r
    switch is not used.  Regional attributes and area constraints are
    propagated in the same manner as holes; you specify a point for each
    attribute and/or constraint, and the attribute and/or constraint
    affects the whole region (bounded by segments) containing the point.
    If two values are written on a line after the x and y coordinate, the
    first such value is assumed to be a regional attribute (but is only
    applied if the -A switch is selected), and the second value is assumed
    to be a regional area constraint (but is only applied if the -a switch
    is selected).  You may specify just one value after the coordinates,
    which can serve as both an attribute and an area constraint, depending
    on the choice of switches.  If you are using the -A and -a switches
    simultaneously and wish to assign an attribute to some region without
    imposing an area constraint, use a negative maximum area.

    When a triangulation is created from a .poly file, you must either
    enclose the entire region to be triangulated in PSLG segments, or
    use the -c switch, which encloses the convex hull of the input vertex
    set.  If you do not use the -c switch, Triangle eats all triangles that
    are not enclosed by segments; if you are not careful, your whole
    triangulation may be eaten away.  If you do use the -c switch, you can
    still produce concavities by the appropriate placement of holes just
    within the convex hull.

    An ideal PSLG has no intersecting segments, nor any vertices that lie
    upon segments (except, of course, the endpoints of each segment.)  You
    aren't required to make your .poly files ideal, but you should be aware
    of what can go wrong.  Segment intersections are relatively safe--
    Triangle calculates the intersection points for you and adds them to
    the triangulation--as long as your machine's floating-point precision
    doesn't become a problem.  You are tempting the fates if you have three
    segments that cross at the same location, and expect Triangle to figure
    out where the intersection point is.  Thanks to floating-point roundoff
    error, Triangle will probably decide that the three segments intersect
    at three different points, and you will find a minuscule triangle in
    your output--unless Triangle tries to refine the tiny triangle, uses
    up the last bit of machine precision, and fails to terminate at all.
    You're better off putting the intersection point in the input files,
    and manually breaking up each segment into two.  Similarly, if you
    place a vertex at the middle of a segment, and hope that Triangle will
    break up the segment at that vertex, you might get lucky.  On the other
    hand, Triangle might decide that the vertex doesn't lie precisely on
    the segment, and you'll have a needle-sharp triangle in your output--or
    a lot of tiny triangles if you're generating a quality mesh.

    When Triangle reads a .poly file, it also writes a .poly file, which
    includes all edges that are parts of input segments.  If the -c switch
    is used, the output .poly file also includes all of the edges on the
    convex hull.  Hence, the output .poly file is useful for finding edges
    associated with input segments and for setting boundary conditions in
    finite element simulations.  Moreover, you will need it if you plan to
    refine the output mesh, and don't want segments to be missing in later
    triangulations.

  .area files:
    First line:  <# of triangles>
    Following lines:  <triangle #> <maximum area>

    An .area file associates with each triangle a maximum area that is used
    for mesh refinement.  As with other file formats, every triangle must
    be represented, and they must be numbered consecutively.  A triangle
    may be left unconstrained by assigning it a negative maximum area.

  .edge files:
    First line:  <# of edges> <# of boundary markers (0 or 1)>
    Following lines:  <edge #> <endpoint> <endpoint> [boundary marker]

    Endpoints are indices into the corresponding .node file.  Triangle can
    produce .edge files (use the -e switch), but cannot read them.  The
    optional column of boundary markers is suppressed by the -B switch.

    In Voronoi diagrams, one also finds a special kind of edge that is an
    infinite ray with only one endpoint.  For these edges, a different
    format is used:

        <edge #> <endpoint> -1 <direction x> <direction y>

    The `direction' is a floating-point vector that indicates the direction
    of the infinite ray.

  .neigh files:
    First line:  <# of triangles> <# of neighbors per triangle (always 3)>
    Following lines:  <triangle #> <neighbor> <neighbor> <neighbor>

    Neighbors are indices into the corresponding .ele file.  An index of -1
    indicates no neighbor (because the triangle is on an exterior
    boundary).  The first neighbor of triangle i is opposite the first
    corner of triangle i, and so on.

    Triangle can produce .neigh files (use the -n switch), but cannot read
    them.

Boundary Markers:

  Boundary markers are tags used mainly to identify which output vertices
  and edges are associated with which PSLG segment, and to identify which
  vertices and edges occur on a boundary of the triangulation.  A common
  use is to determine where boundary conditions should be applied to a
  finite element mesh.  You can prevent boundary markers from being written
  into files produced by Triangle by using the -B switch.

  The boundary marker associated with each segment in an output .poly file
  and each edge in an output .edge file is chosen as follows:
    - If an output edge is part or all of a PSLG segment with a nonzero
      boundary marker, then the edge is assigned the same marker.
    - Otherwise, if the edge occurs on a boundary of the triangulation
      (including boundaries of holes), then the edge is assigned the marker
      one (1).
    - Otherwise, the edge is assigned the marker zero (0).
  The boundary marker associated with each vertex in an output .node file
  is chosen as follows:
    - If a vertex is assigned a nonzero boundary marker in the input file,
      then it is assigned the same marker in the output .node file.
    - Otherwise, if the vertex lies on a PSLG segment (including the
      segment's endpoints) with a nonzero boundary marker, then the vertex
      is assigned the same marker.  If the vertex lies on several such
      segments, one of the markers is chosen arbitrarily.
    - Otherwise, if the vertex occurs on a boundary of the triangulation,
      then the vertex is assigned the marker one (1).
    - Otherwise, the vertex is assigned the marker zero (0).

  If you want Triangle to determine for you which vertices and edges are on
  the boundary, assign them the boundary marker zero (or use no markers at
  all) in your input files.  In the output files, all boundary vertices,
  edges, and segments are assigned the value one.

Triangulation Iteration Numbers:

  Because Triangle can read and refine its own triangulations, input
  and output files have iteration numbers.  For instance, Triangle might
  read the files mesh.3.node, mesh.3.ele, and mesh.3.poly, refine the
  triangulation, and output the files mesh.4.node, mesh.4.ele, and
  mesh.4.poly.  Files with no iteration number are treated as if
  their iteration number is zero; hence, Triangle might read the file
  points.node, triangulate it, and produce the files points.1.node and
  points.1.ele.

  Iteration numbers allow you to create a sequence of successively finer
  meshes suitable for multigrid methods.  They also allow you to produce a
  sequence of meshes using error estimate-driven mesh refinement.

  If you're not using refinement or quality meshing, and you don't like
  iteration numbers, use the -I switch to disable them.  This switch also
  disables output of .node and .poly files to prevent your input files from
  being overwritten.  (If the input is a .poly file that contains its own
  points, a .node file is written.)

Examples of How to Use Triangle:

  `triangle dots' reads vertices from dots.node, and writes their Delaunay
  triangulation to dots.1.node and dots.1.ele.  (dots.1.node is identical
  to dots.node.)  `triangle -I dots' writes the triangulation to dots.ele
  instead.  (No additional .node file is needed, so none is written.)

  `triangle -pe object.1' reads a PSLG from object.1.poly (and possibly
  object.1.node, if the vertices are omitted from object.1.poly) and writes
  its constrained Delaunay triangulation to object.2.node and object.2.ele.
  The segments are copied to object.2.poly, and all edges are written to
  object.2.edge.

  `triangle -pq31.5a.1 object' reads a PSLG from object.poly (and possibly
  object.node), generates a mesh whose angles are all 31.5 degrees or
  greater and whose triangles all have areas of 0.1 or less, and writes the
  mesh to object.1.node and object.1.ele.  Each segment may be broken up
  into multiple subsegments; these are written to object.1.poly.

  Here is a sample file `box.poly' describing a square with a square hole:

    # A box with eight vertices in 2D, no attributes, one boundary marker.
    8 2 0 1
     # Outer box has these vertices:
     1   0 0   0
     2   0 3   0
     3   3 0   0
     4   3 3   33     # A special marker for this vertex.
     # Inner square has these vertices:
     5   1 1   0
     6   1 2   0
     7   2 1   0
     8   2 2   0
    # Five segments with boundary markers.
    5 1
     1   1 2   5      # Left side of outer box.
     # Square hole has these segments:
     2   5 7   0
     3   7 8   0
     4   8 6   10
     5   6 5   0
    # One hole in the middle of the inner square.
    1
     1   1.5 1.5

  Note that some segments are missing from the outer square, so one must
  use the `-c' switch.  After `triangle -pqc box.poly', here is the output
  file `box.1.node', with twelve vertices.  The last four vertices were
  added to meet the angle constraint.  Vertices 1, 2, and 9 have markers
  from segment 1.  Vertices 6 and 8 have markers from segment 4.  All the
  other vertices but 4 have been marked to indicate that they lie on a
  boundary.

    12  2  0  1
       1    0   0      5
       2    0   3      5
       3    3   0      1
       4    3   3     33
       5    1   1      1
       6    1   2     10
       7    2   1      1
       8    2   2     10
       9    0   1.5    5
      10    1.5   0    1
      11    3   1.5    1
      12    1.5   3    1
    # Generated by triangle -pqc box.poly

  Here is the output file `box.1.ele', with twelve triangles.

    12  3  0
       1     5   6   9
       2    10   3   7
       3     6   8  12
       4     9   1   5
       5     6   2   9
       6     7   3  11
       7    11   4   8
       8     7   5  10
       9    12   2   6
      10     8   7  11
      11     5   1  10
      12     8   4  12
    # Generated by triangle -pqc box.poly

  Here is the output file `box.1.poly'.  Note that segments have been added
  to represent the convex hull, and some segments have been split by newly
  added vertices.  Note also that <# of vertices> is set to zero to
  indicate that the vertices should be read from the .node file.

    0  2  0  1
    12  1
       1     1   9     5
       2     5   7     1
       3     8   7     1
       4     6   8    10
       5     5   6     1
       6     3  10     1
       7     4  11     1
       8     2  12     1
       9     9   2     5
      10    10   1     1
      11    11   3     1
      12    12   4     1
    1
       1   1.5 1.5
    # Generated by triangle -pqc box.poly

Refinement and Area Constraints:

  The -r switch causes a mesh (.node and .ele files) to be read and
  refined.  If the -p switch is also used, a .poly file is read and used to
  specify edges that are constrained and cannot be eliminated (although
  they can be divided into smaller edges) by the refinement process.

  When you refine a mesh, you generally want to impose tighter quality
  constraints.  One way to accomplish this is to use -q with a larger
  angle, or -a followed by a smaller area than you used to generate the
  mesh you are refining.  Another way to do this is to create an .area
  file, which specifies a maximum area for each triangle, and use the -a
  switch (without a number following).  Each triangle's area constraint is
  applied to that triangle.  Area constraints tend to diffuse as the mesh
  is refined, so if there are large variations in area constraint between
  adjacent triangles, you may not get the results you want.

  If you are refining a mesh composed of linear (three-node) elements, the
  output mesh contains all the nodes present in the input mesh, in the same
  order, with new nodes added at the end of the .node file.  However, the
  refinement is not hierarchical: there is no guarantee that each output
  element is contained in a single input element.  Often, output elements
  overlap two input elements, and some input edges are not present in the
  output mesh.  Hence, a sequence of refined meshes forms a hierarchy of
  nodes, but not a hierarchy of elements.  If you refine a mesh of higher-
  order elements, the hierarchical property applies only to the nodes at
  the corners of an element; other nodes may not be present in the refined
  mesh.

  Maximum area constraints in .poly files operate differently from those in
  .area files.  A maximum area in a .poly file applies to the whole
  (segment-bounded) region in which a point falls, whereas a maximum area
  in an .area file applies to only one triangle.  Area constraints in .poly
  files are used only when a mesh is first generated, whereas area
  constraints in .area files are used only to refine an existing mesh, and
  are typically based on a posteriori error estimates resulting from a
  finite element simulation on that mesh.

  `triangle -rq25 object.1' reads object.1.node and object.1.ele, then
  refines the triangulation to enforce a 25 degree minimum angle, and then
  writes the refined triangulation to object.2.node and object.2.ele.

  `triangle -rpaa6.2 z.3' reads z.3.node, z.3.ele, z.3.poly, and z.3.area.
  After reconstructing the mesh and its subsegments, Triangle refines the
  mesh so that no triangle has area greater than 6.2, and furthermore the
  triangles satisfy the maximum area constraints in z.3.area.  No angle
  bound is imposed at all.  The output is written to z.4.node, z.4.ele, and
  z.4.poly.

  The sequence `triangle -qa1 x', `triangle -rqa.3 x.1', `triangle -rqa.1
  x.2' creates a sequence of successively finer meshes x.1, x.2, and x.3,
  suitable for multigrid.

Convex Hulls and Mesh Boundaries:

  If the input is a vertex set (rather than a PSLG), Triangle produces its
  convex hull as a by-product in the output .poly file if you use the -c
  switch.  There are faster algorithms for finding a two-dimensional convex
  hull than triangulation, of course, but this one comes for free.

  If the input is an unconstrained mesh (you are using the -r switch but
  not the -p switch), Triangle produces a list of its boundary edges
  (including hole boundaries) as a by-product when you use the -c switch.
  If you also use the -p switch, the output .poly file contains all the
  segments from the input .poly file as well.

Voronoi Diagrams:

  The -v switch produces a Voronoi diagram, in files suffixed .v.node and
  .v.edge.  For example, `triangle -v points' reads points.node, produces
  its Delaunay triangulation in points.1.node and points.1.ele, and
  produces its Voronoi diagram in points.1.v.node and points.1.v.edge.  The
  .v.node file contains a list of all Voronoi vertices, and the .v.edge
  file contains a list of all Voronoi edges, some of which may be infinite
  rays.  (The choice of filenames makes it easy to run the set of Voronoi
  vertices through Triangle, if so desired.)

  This implementation does not use exact arithmetic to compute the Voronoi
  vertices, and does not check whether neighboring vertices are identical.
  Be forewarned that if the Delaunay triangulation is degenerate or
  near-degenerate, the Voronoi diagram may have duplicate vertices,
  crossing edges, or infinite rays whose direction vector is zero.

  The result is a valid Voronoi diagram only if Triangle's output is a true
  Delaunay triangulation.  The Voronoi output is usually meaningless (and
  may contain crossing edges and other pathology) if the output is a CDT or
  CCDT, or if it has holes or concavities.  If the triangulation is convex
  and has no holes, this can be fixed by using the -L switch to ensure a
  conforming Delaunay triangulation is constructed.

Mesh Topology:

  You may wish to know which triangles are adjacent to a certain Delaunay
  edge in an .edge file, which Voronoi regions are adjacent to a certain
  Voronoi edge in a .v.edge file, or which Voronoi regions are adjacent to
  each other.  All of this information can be found by cross-referencing
  output files with the recollection that the Delaunay triangulation and
  the Voronoi diagram are planar duals.

  Specifically, edge i of an .edge file is the dual of Voronoi edge i of
  the corresponding .v.edge file, and is rotated 90 degrees counterclock-
  wise from the Voronoi edge.  Triangle j of an .ele file is the dual of
  vertex j of the corresponding .v.node file.  Voronoi region k is the dual
  of vertex k of the corresponding .node file.

  Hence, to find the triangles adjacent to a Delaunay edge, look at the
  vertices of the corresponding Voronoi edge.  If the endpoints of a
  Voronoi edge are Voronoi vertices 2 and 6 respectively, then triangles 2
  and 6 adjoin the left and right sides of the corresponding Delaunay edge,
  respectively.  To find the Voronoi regions adjacent to a Voronoi edge,
  look at the endpoints of the corresponding Delaunay edge.  If the
  endpoints of a Delaunay edge are input vertices 7 and 12, then Voronoi
  regions 7 and 12 adjoin the right and left sides of the corresponding
  Voronoi edge, respectively.  To find which Voronoi regions are adjacent
  to each other, just read the list of Delaunay edges.

  Triangle does not write a list of Voronoi regions, but one can be
  reconstructed straightforwardly.  For instance, to find all the edges of
  Voronoi region 1, search the output .edge file for every edge that has
  input vertex 1 as an endpoint.  The corresponding dual edges in the
  output .v.edge file form the boundary of Voronoi region 1.

Quadratic Elements:

  Triangle generates meshes with subparametric quadratic elements if the
  -o2 switch is specified.  Quadratic elements have six nodes per element,
  rather than three.  `Subparametric' means that the edges of the triangles
  are always straight, so that subparametric quadratic elements are
  geometrically identical to linear elements, even though they can be used
  with quadratic interpolating functions.  The three extra nodes of an
  element fall at the midpoints of the three edges, with the fourth, fifth,
  and sixth nodes appearing opposite the first, second, and third corners
  respectively.

Statistics:

  After generating a mesh, Triangle prints a count of the number of
  vertices, triangles, edges, exterior boundary edges (including hole
  boundaries), interior boundary edges, and segments in the output mesh.
  If you've forgotten the statistics for an existing mesh, run Triangle on
  that mesh with the -rNEP switches to read the mesh and print the
  statistics without writing any files.  Use -rpNEP if you've got a .poly
  file for the mesh.

  The -V switch produces extended statistics, including a rough estimate
  of memory use, the number of calls to geometric predicates, and
  histograms of triangle aspect ratios and angles in the mesh.

Exact Arithmetic:

  Triangle uses adaptive exact arithmetic to perform what computational
  geometers call the `orientation' and `incircle' tests.  If the floating-
  point arithmetic of your machine conforms to the IEEE 754 standard (as
  most workstations do), and does not use extended precision internal
  floating-point registers, then your output is guaranteed to be an
  absolutely true Delaunay or constrained Delaunay triangulation, roundoff
  error notwithstanding.  The word `adaptive' implies that these arithmetic
  routines compute the result only to the precision necessary to guarantee
  correctness, so they are usually nearly as fast as their approximate
  counterparts.

  Pentiums have extended precision floating-point registers.  These must be
  reconfigured so their precision is reduced to memory precision.  Triangle
  does this if it is compiled correctly.  See the makefile for details.

  The exact tests can be disabled with the -X switch.  On most inputs, this
  switch reduces the computation time by about eight percent--it's not
  worth the risk.  There are rare difficult inputs (having many collinear
  and cocircular vertices), however, for which the difference in speed
  could be a factor of two.  Be forewarned that these are precisely the
  inputs most likely to cause errors if you use the -X switch.  Hence, the
  -X switch is not recommended.

  Unfortunately, the exact tests don't solve every numerical problem.
  Exact arithmetic is not used to compute the positions of new vertices,
  because the bit complexity of vertex coordinates would grow without
  bound.  Hence, segment intersections aren't computed exactly; in very
  unusual cases, roundoff error in computing an intersection point might
  actually lead to an inverted triangle and an invalid triangulation.
  (This is one reason to compute your own intersection points in your .poly
  files.)  Similarly, exact arithmetic is not used to compute the vertices
  of the Voronoi diagram.

  Another pair of problems not solved by the exact arithmetic routines is
  underflow and overflow.  If Triangle is compiled for double precision
  arithmetic, I believe that Triangle's geometric predicates work correctly
  if the exponent of every input coordinate falls in the range [-148, 201].
  Underflow can silently prevent the orientation and incircle tests from
  being performed exactly, while overflow typically causes a floating
  exception.

Calling Triangle from Another Program:

  Read the file triangle.h for details.

Troubleshooting:

  Please read this section before mailing me bugs.

  `My output mesh has no triangles!'

    If you're using a PSLG, you've probably failed to specify a proper set
    of bounding segments, or forgotten to use the -c switch.  Or you may
    have placed a hole badly, thereby eating all your triangles.  To test
    these possibilities, try again with the -c and -O switches.
    Alternatively, all your input vertices may be collinear, in which case
    you can hardly expect to triangulate them.

  `Triangle doesn't terminate, or just crashes.'

    Bad things can happen when triangles get so small that the distance
    between their vertices isn't much larger than the precision of your
    machine's arithmetic.  If you've compiled Triangle for single-precision
    arithmetic, you might do better by recompiling it for double-precision.
    Then again, you might just have to settle for more lenient constraints
    on the minimum angle and the maximum area than you had planned.

    You can minimize precision problems by ensuring that the origin lies
    inside your vertex set, or even inside the densest part of your
    mesh.  If you're triangulating an object whose x coordinates all fall
    between 6247133 and 6247134, you're not leaving much floating-point
    precision for Triangle to work with.

    Precision problems can occur covertly if the input PSLG contains two
    segments that meet (or intersect) at an extremely small angle, or if
    such an angle is introduced by the -c switch.  If you don't realize
    that a tiny angle is being formed, you might never discover why
    Triangle is crashing.  To check for this possibility, use the -S switch
    (with an appropriate limit on the number of Steiner points, found by
    trial-and-error) to stop Triangle early, and view the output .poly file
    with Show Me (described below).  Look carefully for regions where dense
    clusters of vertices are forming and for small angles between segments.
    Zoom in closely, as such segments might look like a single segment from
    a distance.

    If some of the input values are too large, Triangle may suffer a
    floating exception due to overflow when attempting to perform an
    orientation or incircle test.  (Read the section on exact arithmetic
    above.)  Again, I recommend compiling Triangle for double (rather
    than single) precision arithmetic.

    Unexpected problems can arise if you use quality meshing (-q, -a, or
    -u) with an input that is not segment-bounded--that is, if your input
    is a vertex set, or you're using the -c switch.  If the convex hull of
    your input vertices has collinear vertices on its boundary, an input
    vertex that you think lies on the convex hull might actually lie just
    inside the convex hull.  If so, an extremely thin triangle is formed by
    the vertex and the convex hull edge beside it.  When Triangle tries to
    refine the mesh to enforce angle and area constraints, extremely tiny
    triangles may be formed, or Triangle may fail because of insufficient
    floating-point precision.

  `The numbering of the output vertices doesn't match the input vertices.'

    You may have had duplicate input vertices, or you may have eaten some
    of your input vertices with a hole, or by placing them outside the area
    enclosed by segments.  In any case, you can solve the problem by not
    using the -j switch.

  `Triangle executes without incident, but when I look at the resulting
  mesh, it has overlapping triangles or other geometric inconsistencies.'

    If you select the -X switch, Triangle occasionally makes mistakes due
    to floating-point roundoff error.  Although these errors are rare,
    don't use the -X switch.  If you still have problems, please report the
    bug.

  Strange things can happen if you've taken liberties with your PSLG.  Do
  you have a vertex lying in the middle of a segment?  Triangle sometimes
  copes poorly with that sort of thing.  Do you want to lay out a collinear
  row of evenly spaced, segment-connected vertices?  Have you simply
  defined one long segment connecting the leftmost vertex to the rightmost
  vertex, and a bunch of vertices lying along it?  This method occasionally
  works, especially with horizontal and vertical lines, but often it
  doesn't, and you'll have to connect each adjacent pair of vertices with a
  separate segment.  If you don't like it, tough.

  Furthermore, if you have segments that intersect other than at their
  endpoints, try not to let the intersections fall extremely close to PSLG
  vertices or each other.

  If you have problems refining a triangulation not produced by Triangle:
  Are you sure the triangulation is geometrically valid?  Is it formatted
  correctly for Triangle?  Are the triangles all listed so the first three
  vertices are their corners in counterclockwise order?  Are all of the
  triangles constrained Delaunay?  Triangle's Delaunay refinement algorithm
  assumes that it starts with a CDT.

Show Me:

  Triangle comes with a separate program named `Show Me', whose primary
  purpose is to draw meshes on your screen or in PostScript.  Its secondary
  purpose is to check the validity of your input files, and do so more
  thoroughly than Triangle does.  Unlike Triangle, Show Me requires that
  you have the X Windows system.

Triangle on the Web:

  To see an illustrated, updated version of these instructions, check out

    http://www.cs.cmu.edu/~quake/triangle.html

A Brief Plea:

  If you use Triangle, and especially if you use it to accomplish real
  work, I would like very much to hear from you.  A short letter or email
  (to jrs@cs.berkeley.edu) describing how you use Triangle will mean a lot
  to me.  The more people I know are using this program, the more easily I
  can justify spending time on improvements, which in turn will benefit
  you.  Also, I can put you on a list to receive email whenever a new
  version of Triangle is available.

  If you use a mesh generated by Triangle in a publication, please include
  an acknowledgment as well.

Research credit:

  Of course, I can take credit for only a fraction of the ideas that made
  this mesh generator possible.  Triangle owes its existence to the efforts
  of many fine computational geometers and other researchers, including
  Marshall Bern, L. Paul Chew, Boris Delaunay, Rex A. Dwyer, David
  Eppstein, Steven Fortune, Leonidas J. Guibas, Donald E. Knuth, C. L.
  Lawson, Der-Tsai Lee, Ernst P. Mucke, Douglas M. Priest, Jim Ruppert,
  Isaac Saias, Bruce J. Schachter, Micha Sharir, Daniel D. Sleator, Jorge
  Stolfi, Robert E. Tarjan, Christopher J. Van Wyk, and Binhai Zhu.  See
  the comments at the beginning of the source code for references.


/*****************************************************************************/
/*                                                                           */
/*  (triangle.h)                                                             */
/*                                                                           */
/*  Include file for programs that call Triangle.                            */
/*                                                                           */
/*  Accompanies Triangle Versions 1.3 and 1.4                                */
/*  July 19, 1996                                                            */
/*                                                                           */
/*  Copyright 1996                                                           */
/*  Jonathan Richard Shewchuk                                                */
/*  2360 Woolsey #H                                                          */
/*  Berkeley, California  94705-1927                                         */
/*  jrs@cs.berkeley.edu                                                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  How to call Triangle from another program                                */
/*                                                                           */
/*                                                                           */
/*  If you haven't read Triangle's instructions (run "triangle -h" to read   */
/*  them), you won't understand what follows.                                */
/*                                                                           */
/*  Triangle must be compiled into an object file (triangle.o) with the      */
/*  TRILIBRARY symbol defined (preferably by using the -DTRILIBRARY compiler */
/*  switch).  The makefile included with Triangle will do this for you if    */
/*  you run "make trilibrary".  The resulting object file can be called via  */
/*  the procedure triangulate().                                             */
/*                                                                           */
/*  If the size of the object file is important to you, you may wish to      */
/*  generate a reduced version of triangle.o.  The REDUCED symbol gets rid   */
/*  of all features that are primarily of research interest.  Specifically,  */
/*  the -DREDUCED switch eliminates Triangle's -i, -F, -s, and -C switches.  */
/*  The CDT_ONLY symbol gets rid of all meshing algorithms above and beyond  */
/*  constrained Delaunay triangulation.  Specifically, the -DCDT_ONLY switch */
/*  eliminates Triangle's -r, -q, -a, -S, and -s switches.                   */
/*                                                                           */
/*  IMPORTANT:  These definitions (TRILIBRARY, REDUCED, CDT_ONLY) must be    */
/*  made in the makefile or in triangle.c itself.  Putting these definitions */
/*  in this file will not create the desired effect.                         */
/*                                                                           */
/*                                                                           */
/*  The calling convention for triangulate() follows.                        */
/*                                                                           */
/*      void triangulate(triswitches, in, out, vorout)                       */
/*      char *triswitches;                                                   */
/*      struct triangulateio *in;                                            */
/*      struct triangulateio *out;                                           */
/*      struct triangulateio *vorout;                                        */
/*                                                                           */
/*  `triswitches' is a string containing the command line switches you wish  */
/*  to invoke.  No initial dash is required.  Some suggestions:              */
/*                                                                           */
/*  - You'll probably find it convenient to use the `z' switch so that       */
/*    points (and other items) are numbered from zero.  This simplifies      */
/*    indexing, because the first item of any type always starts at index    */
/*    [0] of the corresponding array, whether that item's number is zero or  */
/*    one.                                                                   */
/*  - You'll probably want to use the `Q' (quiet) switch in your final code, */
/*    but you can take advantage of Triangle's printed output (including the */
/*    `V' switch) while debugging.                                           */
/*  - If you are not using the `q' or `a' switches, then the output points   */
/*    will be identical to the input points, except possibly for the         */
/*    boundary markers.  If you don't need the boundary markers, you should  */
/*    use the `N' (no nodes output) switch to save memory.  (If you do need  */
/*    boundary markers, but need to save memory, a good nasty trick is to    */
/*    set out->pointlist equal to in->pointlist before calling triangulate(),*/
/*    so that Triangle overwrites the input points with identical copies.)   */
/*  - The `I' (no iteration numbers) and `g' (.off file output) switches     */
/*    have no effect when Triangle is compiled with TRILIBRARY defined.      */
/*                                                                           */
/*  `in', `out', and `vorout' are descriptions of the input, the output,     */
/*  and the Voronoi output.  If the `v' (Voronoi output) switch is not used, */
/*  `vorout' may be NULL.  `in' and `out' may never be NULL.                 */
/*                                                                           */
/*  Certain fields of the input and output structures must be initialized,   */
/*  as described below.                                                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  The `triangulateio' structure.                                           */
/*                                                                           */
/*  Used to pass data into and out of the triangulate() procedure.           */
/*                                                                           */
/*                                                                           */
/*  Arrays are used to store points, triangles, markers, and so forth.  In   */
/*  all cases, the first item in any array is stored starting at index [0].  */
/*  However, that item is item number `1' unless the `z' switch is used, in  */
/*  which case it is item number `0'.  Hence, you may find it easier to      */
/*  index points (and triangles in the neighbor list) if you use the `z'     */
/*  switch.  Unless, of course, you're calling Triangle from a Fortran       */
/*  program.                                                                 */
/*                                                                           */
/*  Description of fields (except the `numberof' fields, which are obvious): */
/*                                                                           */
/*  `pointlist':  An array of point coordinates.  The first point's x        */
/*    coordinate is at index [0] and its y coordinate at index [1], followed */
/*    by the coordinates of the remaining points.  Each point occupies two   */
/*    REALs.                                                                 */
/*  `pointattributelist':  An array of point attributes.  Each point's       */
/*    attributes occupy `numberofpointattributes' REALs.                     */
/*  `pointmarkerlist':  An array of point markers; one int per point.        */
/*                                                                           */
/*  `trianglelist':  An array of triangle corners.  The first triangle's     */
/*    first corner is at index [0], followed by its other two corners in     */
/*    counterclockwise order, followed by any other nodes if the triangle    */
/*    represents a nonlinear element.  Each triangle occupies                */
/*    `numberofcorners' ints.                                                */
/*  `triangleattributelist':  An array of triangle attributes.  Each         */
/*    triangle's attributes occupy `numberoftriangleattributes' REALs.       */
/*  `trianglearealist':  An array of triangle area constraints; one REAL per */
/*    triangle.  Input only.                                                 */
/*  `neighborlist':  An array of triangle neighbors; three ints per          */
/*    triangle.  Output only.                                                */
/*                                                                           */
/*  `segmentlist':  An array of segment endpoints.  The first segment's      */
/*    endpoints are at indices [0] and [1], followed by the remaining        */
/*    segments.  Two ints per segment.                                       */
/*  `segmentmarkerlist':  An array of segment markers; one int per segment.  */
/*                                                                           */
/*  `holelist':  An array of holes.  The first hole's x and y coordinates    */
/*    are at indices [0] and [1], followed by the remaining holes.  Two      */
/*    REALs per hole.  Input only, although the pointer is copied to the     */
/*    output structure for your convenience.                                 */
/*                                                                           */
/*  `regionlist':  An array of regional attributes and area constraints.     */
/*    The first constraint's x and y coordinates are at indices [0] and [1], */
/*    followed by the regional attribute and index [2], followed by the      */
/*    maximum area at index [3], followed by the remaining area constraints. */
/*    Four REALs per area constraint.  Note that each regional attribute is  */
/*    used only if you select the `A' switch, and each area constraint is    */
/*    used only if you select the `a' switch (with no number following), but */
/*    omitting one of these switches does not change the memory layout.      */
/*    Input only, although the pointer is copied to the output structure for */
/*    your convenience.                                                      */
/*                                                                           */
/*  `edgelist':  An array of edge endpoints.  The first edge's endpoints are */
/*    at indices [0] and [1], followed by the remaining edges.  Two ints per */
/*    edge.  Output only.                                                    */
/*  `edgemarkerlist':  An array of edge markers; one int per edge.  Output   */
/*    only.                                                                  */
/*  `normlist':  An array of normal vectors, used for infinite rays in       */
/*    Voronoi diagrams.  The first normal vector's x and y magnitudes are    */
/*    at indices [0] and [1], followed by the remaining vectors.  For each   */
/*    finite edge in a Voronoi diagram, the normal vector written is the     */
/*    zero vector.  Two REALs per edge.  Output only.                        */
/*                                                                           */
/*                                                                           */
/*  Any input fields that Triangle will examine must be initialized.         */
/*  Furthermore, for each output array that Triangle will write to, you      */
/*  must either provide space by setting the appropriate pointer to point    */
/*  to the space you want the data written to, or you must initialize the    */
/*  pointer to NULL, which tells Triangle to allocate space for the results. */
/*  The latter option is preferable, because Triangle always knows exactly   */
/*  how much space to allocate.  The former option is provided mainly for    */
/*  people who need to call Triangle from Fortran code, though it also makes */
/*  possible some nasty space-saving tricks, like writing the output to the  */
/*  same arrays as the input.                                                */
/*                                                                           */
/*  Triangle will not free() any input or output arrays, including those it  */
/*  allocates itself; that's up to you.                                      */
/*                                                                           */
/*  Here's a guide to help you decide which fields you must initialize       */
/*  before you call triangulate().                                           */
/*                                                                           */
/*  `in':                                                                    */
/*                                                                           */
/*    - `pointlist' must always point to a list of points; `numberofpoints'  */
/*      and `numberofpointattributes' must be properly set.                  */
/*      `pointmarkerlist' must either be set to NULL (in which case all      */
/*      markers default to zero), or must point to a list of markers.  If    */
/*      `numberofpointattributes' is not zero, `pointattributelist' must     */
/*      point to a list of point attributes.                                 */
/*    - If the `r' switch is used, `trianglelist' must point to a list of    */
/*      triangles, and `numberoftriangles', `numberofcorners', and           */
/*      `numberoftriangleattributes' must be properly set.  If               */
/*      `numberoftriangleattributes' is not zero, `triangleattributelist'    */
/*      must point to a list of triangle attributes.  If the `a' switch is   */
/*      used (with no number following), `trianglearealist' must point to a  */
/*      list of triangle area constraints.  `neighborlist' may be ignored.   */
/*    - If the `p' switch is used, `segmentlist' must point to a list of     */
/*      segments, `numberofsegments' must be properly set, and               */
/*      `segmentmarkerlist' must either be set to NULL (in which case all    */
/*      markers default to zero), or must point to a list of markers.        */
/*    - If the `p' switch is used without the `r' switch, then               */
/*      `numberofholes' and `numberofregions' must be properly set.  If      */
/*      `numberofholes' is not zero, `holelist' must point to a list of      */
/*      holes.  If `numberofregions' is not zero, `regionlist' must point to */
/*      a list of region constraints.                                        */
/*    - If the `p' switch is used, `holelist', `numberofholes',              */
/*      `regionlist', and `numberofregions' is copied to `out'.  (You can    */
/*      nonetheless get away with not initializing them if the `r' switch is */
/*      used.)                                                               */
/*    - `edgelist', `edgemarkerlist', `normlist', and `numberofedges' may be */
/*      ignored.                                                             */
/*                                                                           */
/*  `out':                                                                   */
/*                                                                           */
/*    - `pointlist' must be initialized (NULL or pointing to memory) unless  */
/*      the `N' switch is used.  `pointmarkerlist' must be initialized       */
/*      unless the `N' or `B' switch is used.  If `N' is not used and        */
/*      `in->numberofpointattributes' is not zero, `pointattributelist' must */
/*      be initialized.                                                      */
/*    - `trianglelist' must be initialized unless the `E' switch is used.    */
/*      `neighborlist' must be initialized if the `n' switch is used.  If    */
/*      the `E' switch is not used and (`in->numberofelementattributes' is   */
/*      not zero or the `A' switch is used), `elementattributelist' must be  */
/*      initialized.  `trianglearealist' may be ignored.                     */
/*    - `segmentlist' must be initialized if the `p' or `c' switch is used,  */
/*      and the `P' switch is not used.  `segmentmarkerlist' must also be    */
/*      initialized under these circumstances unless the `B' switch is used. */
/*    - `edgelist' must be initialized if the `e' switch is used.            */
/*      `edgemarkerlist' must be initialized if the `e' switch is used and   */
/*      the `B' switch is not.                                               */
/*    - `holelist', `regionlist', `normlist', and all scalars may be ignored.*/
/*                                                                           */
/*  `vorout' (only needed if `v' switch is used):                            */
/*                                                                           */
/*    - `pointlist' must be initialized.  If `in->numberofpointattributes'   */
/*      is not zero, `pointattributelist' must be initialized.               */
/*      `pointmarkerlist' may be ignored.                                    */
/*    - `edgelist' and `normlist' must both be initialized.                  */
/*      `edgemarkerlist' may be ignored.                                     */
/*    - Everything else may be ignored.                                      */
/*                                                                           */
/*  After a call to triangulate(), the valid fields of `out' and `vorout'    */
/*  will depend, in an obvious way, on the choice of switches used.  Note    */
/*  that when the `p' switch is used, the pointers `holelist' and            */
/*  `regionlist' are copied from `in' to `out', but no new space is          */
/*  allocated; be careful that you don't free() the same array twice.  On    */
/*  the other hand, Triangle will never copy the `pointlist' pointer (or any */
/*  others); new space is allocated for `out->pointlist', or if the `N'      */
/*  switch is used, `out->pointlist' remains uninitialized.                  */
/*                                                                           */
/*  All of the meaningful `numberof' fields will be properly set; for        */
/*  instance, `numberofedges' will represent the number of edges in the      */
/*  triangulation whether or not the edges were written.  If segments are    */
/*  not used, `numberofsegments' will indicate the number of boundary edges. */
/*                                                                           */
/*****************************************************************************/

#ifndef REAL
#define REAL float
#endif

#define ANSI_DECLARATORS

struct triangulateio {
  REAL *pointlist;                                               /* In / out */
  REAL *pointattributelist;                                      /* In / out */
  int *pointmarkerlist;                                          /* In / out */
  int numberofpoints;                                            /* In / out */
  int numberofpointattributes;                                   /* In / out */

  int *trianglelist;                                             /* In / out */
  REAL *triangleattributelist;                                   /* In / out */
  REAL *trianglearealist;                                         /* In only */
  int *neighborlist;                                             /* Out only */
  int numberoftriangles;                                         /* In / out */
  int numberofcorners;                                           /* In / out */
  int numberoftriangleattributes;                                /* In / out */

  int *segmentlist;                                              /* In / out */
  int *segmentmarkerlist;                                        /* In / out */
  int numberofsegments;                                          /* In / out */

  REAL *holelist;                        /* In / pointer to array copied out */
  int numberofholes;                                      /* In / copied out */

  REAL *regionlist;                      /* In / pointer to array copied out */
  int numberofregions;                                    /* In / copied out */

  int *edgelist;                                                 /* Out only */
  int *edgemarkerlist;            /* Not used with Voronoi diagram; out only */
  REAL *normlist;                /* Used only with Voronoi diagram; out only */
  int numberofedges;                                             /* Out only */
};

#ifdef ANSI_DECLARATORS
void triangulate(char *, struct triangulateio *, struct triangulateio *,
                 struct triangulateio *);
#else /* not ANSI_DECLARATORS */
void triangulate();
#endif /* not ANSI_DECLARATORS */
