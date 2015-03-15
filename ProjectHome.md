# Introduction #

"**PR4PR**" is short for "_Particle Resolver for Pixar Renderman_", a DSO for [PIXAR RenderMan](http://www.pixar.com) designed for particle rendering.

It supplies some useful features for particle rendering,
  * interpolate massive points and attributes from Maya's seed particle shape,
  * load external Karakatoa .prt file from 3dsmax and feed it into [PIXAR RenderMan](http://www.pixar.com),
  * Run your procedural primitive DSOs or executable programs.

# Summary #

As we known, the Maya's built-in particle engine is very limted until now compared with XSI ICE or Houdini, and there are some amazing volume renderer at production level such as Karakatoa in 3dsmax, 5x5 for Houdini, and DD STORM.

For historical reason, PIXAR choosed Maya as their basic workflow framework, near everything is almost based on Maya not 3dsmax or XSI (they own an inner animation tool). In order to improve the particle part, I decided to make a flexiable DSO for rendering particles only.

# Usage #

PR4PR is a RifPlugin, so that all the functions are controlled by attributes in RIB, take an example

```
		#slim ribbox RIBBox 
		Attribute "user" "uniform int PRType" [ 1 ]
		Attribute "user" "uniform int NCopies" [ 1024 ]
		Attribute "user" "uniform int RandPattern" [ 1 ]
		Attribute "user" "uniform int Seed" [183]
		Points "P" # Others data here
```

"PRType" means which PR you want to use, currently **1** is the DPR, **0** means not RenderMan built-in routines without any DSOs.

# Gallery #
![http://images.cnblogs.com/cnblogs_com/Jedimaster/WindowsLiveWriter/DiffusionParticleResolver_13ECE/DPR_1_O.png](http://images.cnblogs.com/cnblogs_com/Jedimaster/WindowsLiveWriter/DiffusionParticleResolver_13ECE/DPR_1_O.png)

![http://images.cnblogs.com/cnblogs_com/Jedimaster/WindowsLiveWriter/DiffusionParticleResolver_13ECE/DPR_1_D.png](http://images.cnblogs.com/cnblogs_com/Jedimaster/WindowsLiveWriter/DiffusionParticleResolver_13ECE/DPR_1_D.png)