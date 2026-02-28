# Colormap Visualization Controls

## Colormap Selection
Press number keys to switch between different colormaps:
- **1**: Viridis (perceptually uniform, default)
- **2**: Turbo (improved rainbow)
- **3**: Jet (classic rainbow)

## Scalar Range Adjustment
Interactively adjust the visible scalar value range:
- **Q**: Decrease minimum range value (step: -0.05)
- **W**: Increase minimum range value (step: +0.05)
- **E**: Decrease maximum range value (step: -0.05)
- **R**: Increase maximum range value (step: +0.05)

## Wireframe Overlay
Toggle and adjust wireframe overlay without a second draw call:
- **T**: Toggle wireframe overlay on/off
- **Y**: Decrease wireframe thickness (step: -0.2)
- **U**: Increase wireframe thickness (step: +0.2)

The wireframe is rendered using barycentric coordinates in the fragment shader, allowing single-pass rendering with both solid mesh and wireframe overlay.

## Mesh Displacement
Apply runtime displacement using per-vertex vector field:
- **I**: Decrease displacement scale (step: -0.5)
- **O**: Increase displacement scale (step: +0.5)

Each vertex has a displacement vector calculated as:
```
displacement = vec3(cos(x*5)*0.1, sin(y*5)*0.1, scalarValue*0.05)
```
The displacement creates a wave-like deformation based on position and scalar value. Scale range: 0.0 - 10.0

## GPU Compute Shaders
Use GPU compute shaders for advanced processing:
- **C**: Toggle GPU-based normal recomputation (ON/OFF)

When compute normals is enabled:
- A compute shader recalculates surface normals after displacement
- Provides accurate lighting on deformed mesh
- Runs before each frame render
- Uses OpenGL Shader Storage Buffer Objects (SSBO)
- Processes 29,400 vertices using work groups of 256 threads

The compute shader automatically:
1. Applies displacement to each vertex position
2. Computes face normals from displaced triangles
3. Updates vertex normals for correct lighting
4. Synchronizes with rendering pipeline

**Note**: Only active when displacement scale > 0.0

## Isolines (Contour Lines)
Display contour lines at regular scalar value intervals:
- **P**: Toggle isolines on/off
- **[ (Left Bracket)**: Decrease isoline interval (step: -0.05, range: 0.05-2.0)
- **] (Right Bracket)**: Increase isoline interval (step: +0.05, range: 0.05-2.0)
- **; (Semicolon)**: Decrease isoline thickness (step: -0.3, range: 0.5-5.0)
- **' (Apostrophe)**: Increase isoline thickness (step: +0.3, range: 0.5-5.0)

Isolines are computed directly in the fragment shader using the normalized scalar value (0.0-1.0 range):
```glsl
float normalizedValue = (scalarValue - minVal) / (maxVal - minVal);
float modValue = mod(normalizedValue, isolineInterval);
float minDist = min(modValue, isolineInterval - modValue);
```

**Default settings**: Interval = 0.1 (10 contour lines), Thickness = 2.0

This creates black contour lines at regular intervals (e.g., 0.0, 0.1, 0.2, 0.3, ... 1.0), useful for visualizing gradient changes and identifying regions of equal scalar values. The lines are anti-aliased and screen-space adaptive for smooth rendering at any zoom level.

## Out-of-Range Values
Values outside the selected range are discarded (not rendered).

## Camera Controls
- **Left Mouse Button + Drag**: Orbit camera around world origin (0, 0, 0)
- **Mouse Scroll**: Zoom in/out (adjust FOV)

## Scalar Value Generation
Each vertex has a synthetic scalar value calculated as:
```
f(x, y, z) = sin(x * 10) * cos(y * 10) + noise
```
Where noise is a random value in the range [-0.1, 0.1]

The full scalar range is approximately [-1.2, 1.2], which is normalized to [0.0, 1.0] for colormap application.

## Mesh Generation Methods

### CreateRectangle()
- Generates a subdivided grid with **5,041 vertices** (71 × 71)
- Uses **indexed rendering** with 9,800 triangles
- Optimized for memory efficiency through vertex sharing
- Barycentric coordinates: Not included (vertices are shared)

### CreateRectangleTriangleList()
- Generates a triangle list with **29,400 vertices** (4,900 quads × 6 vertices)
- Uses **non-indexed rendering** (glDrawArrays)
- Each triangle has independent vertices (no vertex sharing)
- **Barycentric coordinates included**: Each vertex has (1,0,0), (0,1,0), or (0,0,1)
- Useful for applications requiring per-triangle data, wireframe rendering, or geometry shaders
- Available at vertex shader location 4 and fragment shader as `BarycentricFrag`

### Barycentric Coordinates
Barycentric coordinates uniquely identify a point within a triangle:
- Vertex 0: (1, 0, 0)
- Vertex 1: (0, 1, 0)
- Vertex 2: (0, 0, 1)

These can be used for:
- Wireframe rendering without geometry shaders
- Per-triangle effects
- Edge detection
- Advanced shading techniques


