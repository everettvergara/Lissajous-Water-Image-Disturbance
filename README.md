
An odd way to animate water in a 2D still photo using Lissajous Curves and SDL.

Lissajous Curves
---

In animating a point moving along the perimiter of the circle, we do:

```c++
    int x = cx + radius * cos(angle_in_radians);
    int y = cy + radius * sin(angle_in_radians);
```

In Lissajous curves, we simply parameterized everything. We also introduced the element of randomness as initial values to give that "fly" effect. The overall output is amazingly beautiful.

```c++
    int x = cx + radius_x * cos(angle_in_radians_x);
    int y = cy + radius_y * sin(angle_in_radians_y);
```

![alt text](https://github.com/everettvergara/Lissajous-Image-Water-Disturbance/blob/main/assets/Water-Disturbance.png)