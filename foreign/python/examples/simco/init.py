import softpy

Initial = softpy.entity(open('initial.json'))

i = Initial(dimension_sizes=[50,50])
i.dt = 2.0e-3
i.startt = 0.0
i.endt = 2.0
i.nu = 1.0e-2
i.sample_interval = 1.0

s = softpy.Storage(driver='hdf5', uri='init.h5')
s.save(i)
s.close()
print(softpy.get_id(i));
