#Raytra

A simple ray tracer written in C++

<hr>

### Compile

```
git clone https://github.com/bahuljain/Raytra.git
cd Raytra
make
```

### Run

```
./prog_out <scene_file_name> <output_image_name.exr> <primary_ray_samples> <shadow_ray_samples> [mode]
```

#### Run Modes

- empty - render using acceleration structures
- 0     - render without using acceleration structures (this could take a lot of time if the scene has a lot of surfaces).
- 1     - render the bounding boxes of the surface instead of the surface itself.

### Run Tests

```
make test
```

### View Rendered Image

Using exr-viewer:
```
exrdisplay <output_image_name.exr>
```

On Mac:
```
open <output_image_name.exr>
```