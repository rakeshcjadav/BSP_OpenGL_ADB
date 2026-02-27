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

## Out-of-Range Values
Values outside the selected range are displayed in distinct colors:
- **Below minimum**: Dark gray (0.2, 0.2, 0.2)
- **Above maximum**: Light gray (0.8, 0.8, 0.8)

## Camera Controls
- **Left Mouse Button + Drag**: Orbit camera around world origin (0, 0, 0)
- **Mouse Scroll**: Zoom in/out (adjust FOV)

## Scalar Value Generation
Each vertex has a synthetic scalar value calculated as:
```
f(x, y, z) = sin(x) * cos(z) + noise
```
Where noise is a random value in the range [-0.1, 0.1]

The full scalar range is approximately [-1.2, 1.2], which is normalized to [0.0, 1.0] for colormap application.
