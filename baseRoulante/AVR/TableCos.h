#define PI 3.141593
#define NOMBRE_COS 1000
#define COS_LIST 1.000000, 0.999995, 0.999980, 0.999955, 0.999921, 0.999876, 0.999822, 0.999758, 0.999684, 0.999600, 0.999506, 0.999402, 0.999288, 0.999164, 0.999031, 0.998888, 0.998734, 0.998571, 0.998398, 0.998215, 0.998023, 0.997820, 0.997608, 0.997385, 0.997153, 0.996911, 0.996659, 0.996397, 0.996126, 0.995844, 0.995553, 0.995252, 0.994941, 0.994620, 0.994289, 0.993949, 0.993599, 0.993238, 0.992868, 0.992489, 0.992099, 0.991699, 0.991290, 0.990871, 0.990442, 0.990004, 0.989555, 0.989097, 0.988629, 0.988151, 0.987664, 0.987166, 0.986659, 0.986142, 0.985616, 0.985080, 0.984533, 0.983978, 0.983412, 0.982837, 0.982252, 0.981657, 0.981053, 0.980439, 0.979815, 0.979181, 0.978538, 0.977885, 0.977223, 0.976551, 0.975869, 0.975177, 0.974476, 0.973765, 0.973045, 0.972315, 0.971575, 0.970826, 0.970067, 0.969299, 0.968521, 0.967733, 0.966936, 0.966129, 0.965313, 0.964487, 0.963651, 0.962807, 0.961952, 0.961088, 0.960215, 0.959332, 0.958439, 0.957537, 0.956626, 0.955705, 0.954775, 0.953835, 0.952886, 0.951927, 0.950959, 0.949982, 0.948995, 0.947999, 0.946993, 0.945978, 0.944954, 0.943920, 0.942877, 0.941825, 0.940764, 0.939693, 0.938612, 0.937523, 0.936424, 0.935316, 0.934199, 0.933072, 0.931937, 0.930792, 0.929638, 0.928474, 0.927302, 0.926120, 0.924929, 0.923729, 0.922520, 0.921302, 0.920074, 0.918838, 0.917592, 0.916338, 0.915074, 0.913801, 0.912519, 0.911228, 0.909929, 0.908620, 0.907302, 0.905975, 0.904640, 0.903295, 0.901941, 0.900579, 0.899207, 0.897827, 0.896438, 0.895040, 0.893633, 0.892217, 0.890792, 0.889359, 0.887917, 0.886466, 0.885006, 0.883537, 0.882060, 0.880574, 0.879080, 0.877576, 0.876064, 0.874543, 0.873014, 0.871476, 0.869930, 0.868374, 0.866811, 0.865238, 0.863657, 0.862068, 0.860470, 0.858863, 0.857248, 0.855625, 0.853993, 0.852352, 0.850704, 0.849046, 0.847381, 0.845707, 0.844024, 0.842334, 0.840635, 0.838927, 0.837212, 0.835488, 0.833756, 0.832015, 0.830267, 0.828510, 0.826745, 0.824971, 0.823190, 0.821400, 0.819603, 0.817797, 0.815983, 0.814161, 0.812331, 0.810493, 0.808647, 0.806793, 0.804931, 0.803061, 0.801183, 0.799297, 0.797404, 0.795502, 0.793593, 0.791675, 0.789750, 0.787817, 0.785876, 0.783928, 0.781972, 0.780007, 0.778036, 0.776056, 0.774069, 0.772074, 0.770072, 0.768062, 0.766044, 0.764019, 0.761987, 0.759946, 0.757898, 0.755843, 0.753780, 0.751710, 0.749633, 0.747547, 0.745455, 0.743355, 0.741248, 0.739134, 0.737012, 0.734883, 0.732746, 0.730603, 0.728452, 0.726294, 0.724128, 0.721956, 0.719777, 0.717590, 0.715396, 0.713195, 0.710987, 0.708773, 0.706551, 0.704322, 0.702086, 0.699843, 0.697593, 0.695337, 0.693073, 0.690803, 0.688526, 0.686242, 0.683951, 0.681653, 0.679349, 0.677038, 0.674720, 0.672396, 0.670065, 0.667727, 0.665383, 0.663032, 0.660675, 0.658311, 0.655940, 0.653563, 0.651180, 0.648790, 0.646394, 0.643991, 0.641582, 0.639167, 0.636745, 0.634317, 0.631883, 0.629443, 0.626996, 0.624543, 0.622084, 0.619619, 0.617147, 0.614670, 0.612186, 0.609697, 0.607201, 0.604699, 0.602192, 0.599678, 0.597159, 0.594633, 0.592102, 0.589565, 0.587022, 0.584473, 0.581918, 0.579358, 0.576792, 0.574220, 0.571643, 0.569060, 0.566471, 0.563877, 0.561277, 0.558671, 0.556060, 0.553444, 0.550822, 0.548195, 0.545562, 0.542924, 0.540280, 0.537631, 0.534977, 0.532317, 0.529653, 0.526983, 0.524307, 0.521627, 0.518941, 0.516251, 0.513555, 0.510854, 0.508148, 0.505437, 0.502721, 0.500000, 0.497274, 0.494543, 0.491808, 0.489067, 0.486322, 0.483571, 0.480816, 0.478057, 0.475292, 0.472523, 0.469749, 0.466971, 0.464188, 0.461400, 0.458608, 0.455811, 0.453010, 0.450204, 0.447394, 0.444579, 0.441760, 0.438936, 0.436109, 0.433277, 0.430440, 0.427600, 0.424755, 0.421906, 0.419052, 0.416195, 0.413334, 0.410468, 0.407598, 0.404725, 0.401847, 0.398965, 0.396080, 0.393190, 0.390297, 0.387400, 0.384499, 0.381594, 0.378685, 0.375773, 0.372856, 0.369937, 0.367013, 0.364086, 0.361155, 0.358221, 0.355283, 0.352342, 0.349397, 0.346449, 0.343497, 0.340542, 0.337584, 0.334622, 0.331657, 0.328688, 0.325717, 0.322742, 0.319764, 0.316783, 0.313798, 0.310811, 0.307820, 0.304827, 0.301830, 0.298831, 0.295828, 0.292823, 0.289814, 0.286803, 0.283789, 0.280772, 0.277753, 0.274730, 0.271705, 0.268678, 0.265647, 0.262614, 0.259578, 0.256540, 0.253499, 0.250456, 0.247410, 0.244362, 0.241312, 0.238259, 0.235203, 0.232146, 0.229086, 0.226023, 0.222959, 0.219892, 0.216823, 0.213752, 0.210679, 0.207604, 0.204527, 0.201448, 0.198366, 0.195283, 0.192198, 0.189111, 0.186022, 0.182931, 0.179839, 0.176744, 0.173648, 0.170550, 0.167451, 0.164350, 0.161247, 0.158143, 0.155037, 0.151929, 0.148820, 0.145710, 0.142598, 0.139485, 0.136370, 0.133254, 0.130136, 0.127018, 0.123898, 0.120777, 0.117655, 0.114531, 0.111406, 0.108281, 0.105154, 0.102026, 0.098897, 0.095767, 0.092637, 0.089505, 0.086373, 0.083239, 0.080105, 0.076970, 0.073834, 0.070698, 0.067560, 0.064422, 0.061284, 0.058145, 0.055005, 0.051865, 0.048724, 0.045583, 0.042441, 0.039299, 0.036157, 0.033014, 0.029871, 0.026727, 0.023583, 0.020439, 0.017295, 0.014151, 0.011006, 0.007862, 0.004717, 0.001572, -0.001572, -0.004717, -0.007862, -0.011006, -0.014151, -0.017295, -0.020439, -0.023583, -0.026727, -0.029871, -0.033014, -0.036157, -0.039299, -0.042441, -0.045583, -0.048724, -0.051865, -0.055005, -0.058145, -0.061284, -0.064422, -0.067560, -0.070698, -0.073834, -0.076970, -0.080105, -0.083239, -0.086373, -0.089505, -0.092637, -0.095767, -0.098897, -0.102026, -0.105154, -0.108281, -0.111406, -0.114531, -0.117655, -0.120777, -0.123898, -0.127018, -0.130136, -0.133254, -0.136370, -0.139485, -0.142598, -0.145710, -0.148820, -0.151929, -0.155037, -0.158143, -0.161247, -0.164350, -0.167451, -0.170550, -0.173648, -0.176744, -0.179839, -0.182931, -0.186022, -0.189111, -0.192198, -0.195283, -0.198366, -0.201448, -0.204527, -0.207604, -0.210679, -0.213752, -0.216823, -0.219892, -0.222959, -0.226023, -0.229086, -0.232146, -0.235203, -0.238259, -0.241312, -0.244362, -0.247410, -0.250456, -0.253499, -0.256540, -0.259578, -0.262614, -0.265647, -0.268678, -0.271705, -0.274730, -0.277753, -0.280772, -0.283789, -0.286803, -0.289814, -0.292823, -0.295828, -0.298831, -0.301830, -0.304827, -0.307820, -0.310811, -0.313798, -0.316783, -0.319764, -0.322742, -0.325717, -0.328688, -0.331657, -0.334622, -0.337584, -0.340542, -0.343497, -0.346449, -0.349397, -0.352342, -0.355283, -0.358221, -0.361155, -0.364086, -0.367013, -0.369937, -0.372856, -0.375773, -0.378685, -0.381594, -0.384499, -0.387400, -0.390297, -0.393190, -0.396080, -0.398965, -0.401847, -0.404725, -0.407598, -0.410468, -0.413334, -0.416195, -0.419052, -0.421906, -0.424755, -0.427600, -0.430440, -0.433277, -0.436109, -0.438936, -0.441760, -0.444579, -0.447394, -0.450204, -0.453010, -0.455811, -0.458608, -0.461400, -0.464188, -0.466971, -0.469749, -0.472523, -0.475292, -0.478057, -0.480816, -0.483571, -0.486322, -0.489067, -0.491808, -0.494543, -0.497274, -0.500000, -0.502721, -0.505437, -0.508148, -0.510854, -0.513555, -0.516251, -0.518941, -0.521627, -0.524307, -0.526983, -0.529653, -0.532317, -0.534977, -0.537631, -0.540280, -0.542924, -0.545562, -0.548195, -0.550822, -0.553444, -0.556060, -0.558671, -0.561277, -0.563877, -0.566471, -0.569060, -0.571643, -0.574220, -0.576792, -0.579358, -0.581918, -0.584473, -0.587022, -0.589565, -0.592102, -0.594633, -0.597159, -0.599678, -0.602192, -0.604699, -0.607201, -0.609697, -0.612186, -0.614670, -0.617147, -0.619619, -0.622084, -0.624543, -0.626996, -0.629443, -0.631883, -0.634317, -0.636745, -0.639167, -0.641582, -0.643991, -0.646394, -0.648790, -0.651180, -0.653563, -0.655940, -0.658311, -0.660675, -0.663032, -0.665383, -0.667727, -0.670065, -0.672396, -0.674720, -0.677038, -0.679349, -0.681653, -0.683951, -0.686242, -0.688526, -0.690803, -0.693073, -0.695337, -0.697593, -0.699843, -0.702086, -0.704322, -0.706551, -0.708773, -0.710987, -0.713195, -0.715396, -0.717590, -0.719777, -0.721956, -0.724128, -0.726294, -0.728452, -0.730603, -0.732746, -0.734883, -0.737012, -0.739134, -0.741248, -0.743355, -0.745455, -0.747547, -0.749633, -0.751710, -0.753780, -0.755843, -0.757898, -0.759946, -0.761987, -0.764019, -0.766044, -0.768062, -0.770072, -0.772074, -0.774069, -0.776056, -0.778036, -0.780007, -0.781972, -0.783928, -0.785876, -0.787817, -0.789750, -0.791675, -0.793593, -0.795502, -0.797404, -0.799297, -0.801183, -0.803061, -0.804931, -0.806793, -0.808647, -0.810493, -0.812331, -0.814161, -0.815983, -0.817797, -0.819603, -0.821400, -0.823190, -0.824971, -0.826745, -0.828510, -0.830267, -0.832015, -0.833756, -0.835488, -0.837212, -0.838927, -0.840635, -0.842334, -0.844024, -0.845707, -0.847381, -0.849046, -0.850704, -0.852352, -0.853993, -0.855625, -0.857248, -0.858863, -0.860470, -0.862068, -0.863657, -0.865238, -0.866811, -0.868374, -0.869930, -0.871476, -0.873014, -0.874543, -0.876064, -0.877576, -0.879080, -0.880574, -0.882060, -0.883537, -0.885006, -0.886466, -0.887917, -0.889359, -0.890792, -0.892217, -0.893633, -0.895040, -0.896438, -0.897827, -0.899207, -0.900579, -0.901941, -0.903295, -0.904640, -0.905975, -0.907302, -0.908620, -0.909929, -0.911228, -0.912519, -0.913801, -0.915074, -0.916338, -0.917592, -0.918838, -0.920074, -0.921302, -0.922520, -0.923729, -0.924929, -0.926120, -0.927302, -0.928474, -0.929638, -0.930792, -0.931937, -0.933072, -0.934199, -0.935316, -0.936424, -0.937523, -0.938612, -0.939693, -0.940764, -0.941825, -0.942877, -0.943920, -0.944954, -0.945978, -0.946993, -0.947999, -0.948995, -0.949982, -0.950959, -0.951927, -0.952886, -0.953835, -0.954775, -0.955705, -0.956626, -0.957537, -0.958439, -0.959332, -0.960215, -0.961088, -0.961952, -0.962807, -0.963651, -0.964487, -0.965313, -0.966129, -0.966936, -0.967733, -0.968521, -0.969299, -0.970067, -0.970826, -0.971575, -0.972315, -0.973045, -0.973765, -0.974476, -0.975177, -0.975869, -0.976551, -0.977223, -0.977885, -0.978538, -0.979181, -0.979815, -0.980439, -0.981053, -0.981657, -0.982252, -0.982837, -0.983412, -0.983978, -0.984533, -0.985080, -0.985616, -0.986142, -0.986659, -0.987166, -0.987664, -0.988151, -0.988629, -0.989097, -0.989555, -0.990004, -0.990442, -0.990871, -0.991290, -0.991699, -0.992099, -0.992489, -0.992868, -0.993238, -0.993599, -0.993949, -0.994289, -0.994620, -0.994941, -0.995252, -0.995553, -0.995844, -0.996126, -0.996397, -0.996659, -0.996911, -0.997153, -0.997385, -0.997608, -0.997820, -0.998023, -0.998215, -0.998398, -0.998571, -0.998734, -0.998888, -0.999031, -0.999164, -0.999288, -0.999402, -0.999506, -0.999600, -0.999684, -0.999758, -0.999822, -0.999876, -0.999921, -0.999955, -0.999980, -0.999995, -0.999995