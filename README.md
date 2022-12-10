# Basic knowledge of rendering

In real time rendering, which requires highly-efficient computations, to support rendering smooth video. So, before RTX20, the ray tracing technique, people always use some highly simplified model to describe the world. 

We can imagine camera is a point, and the image which we can see is a projection from 3d space to a 2d space. This part is really similar to pinhole model. Also, we need to determine the color of each point in 3d space which is affeted by lighting. One thing need to emphasize is, we need to computate each point in GPU parallel, this means we should compute each point independently. Specifically, it's means each point wouldn't know information depending on other points.

Now, we will introduce some technology about lighting and shadow.

# Lighting

## Phong lighting model

## Phisically based rendering

### How a rendering model can be called PBR?

* Based on the microfacet surface model
* Be energy connserving
* Use a physical based BRDF(Bidirectional Reflectance Distribution Function)

### Microfacet Surface Model

* In Microfacet surface model, the surface of all materials is considered to be composed of many tiny planes with different orientations. Smoother leads to more order, rougher leads to disorder.
* half angle vector: $h=\frac{\vec{l}+\vec{v}}{\Vert \vec{l}+\vec{v} \Vert}$
* We can use the roughness to describe the probability that the half angle vector coincides with the normal direction of the microfacet. 

### Energy Connserving

* It is obviously that the energy of the incident light whould not be smaller that the reflected light.
* We split the reflected light into two parts: Diffuse and Specular Light.
* Specifically, metal materials have no diffuse reflection, that is, only specular highlights. (This is due to the complex refractive index of the metal, and the light will attenuate quickly after entering the metal, so the scattering part is very small.)

## Physical Based BRDF

(Introduce the BRDF, $L_0(p,\omega_0)$)
$L_o(p,\omega_o)=\int_{\Omega}(k_d\frac{c}{\pi}+k_s\frac{DGF}{4(\vec{\omega_o}\cdot \vec{n})(\vec{\omega_i}\cdot \vec{n})})L_i(p,\omega_i)(\vec{n}\cdot \vec{\omega_i}）d\omega_i$
(省略推导过程。？)

### Normal Distribution Function: $D(\omega_h)$

* $D(\omega_h)$ is the probability density that angle beteewn $\vec{n}$ and $\vec{h}$ equals $\theta$: $D(\omega_h)=\frac{dP}{d\theta}=\frac{\alpha^2}{\pi[(\vec{n}\cdot \vec{h})^2(\alpha^2-1)+1]^2}$
* $\alpha$ is the roughness.

### Geometry Function: $G(\omega_i)$ and $G(\omega_o)$ 

* Geometry function simulates the phenomenon that the mutual occlusion of microfacet leads to the reduction of light energy.
* $G(\vec{n},\vec{v},k)=\frac{\vec{n}\cdot\vec{v}}{(\vec{n}\cdot\vec{v})(1-k)+k}$
* $k$ is calculated by $\alpha$
    For direct light: $k_{direct}=\frac{(\alpha+1)^2}{8}$
    For IBL light: $k_{IBL}=\frac{\alpha^2}{2}$
* $G(\vec{n},\vec{v},\vec{l},k)=G_{sub}(\vec{n},\vec{v},k)G_{sub}(\vec{n},\vec{l},k)$

### Approximation of Fresnel's Formula

* In physics, Fresnel's formula describes the reflectance of ideal specular reflection of light at the interface of a medium.
    We can derive the Fresnel formula through the boundary value relation of the electromagnetic field($E_{1\tau}=E_{2\tau},H_{1\tau}=H_{2\tau}$).
    $r_{\parallel}=\frac{n_t\cos{\theta_i}-n_i\cos{\theta_t}}{n_t\cos{\theta_i}+n_i\cos{\theta_t}}$
    $r_{\perp}=\frac{n_i\cos{\theta_i}-n_t\cos{\theta_t}}{n_i\cos{\theta_i}+n_t\cos{\theta_t}}$
    (These are not very important in this part.)
* Schilick approximation of Fresnel's formula:
    $F(\vec{h},\vec{v},F_0)=F_0+(1-F_0)(1-\vec{h}\cdot \vec{v})^5$

Reference: Karis B, Games E. Real shading in unreal engine 4[J]. Proc. Physically Based Shading Theory Practice, 2013, 4(3): 1.

# Shadow

You can notice, what we have not considered so far is object occlusion. So, there won't be any shadows in our wordls!

## Traditional shadow techinques

Maybe 20 years ago to 10 years ago, even in nowadays, the most popular techinque is shadow map. Due to time issues, we only discuss point light sources today. In this technique, people first move their camera to the point light, and you can render a image which be so-called shadowmap. This image actually is a depth map, like you see in previous class, can tell us which point is nearest to light in every rays. After this, we put the camera back, and we render this whole image by previous steps, but we now know a new information before we start rendering it! we know whether this point is the closest point to the light! So, we can get right shadows!

This technique has a lot of bugs, and too slow! 

* PCF : Percentage-Closer Filtering

* Deferred Rendering

## Soft shadow

But in real world, we know the shadow should be soft. Very intuitive. 

Shadow map also has some techinque to achieve this :

* PCSS : Percentage-Closer Soft Shadow (Based on PCF)
* VSMM : Variance Soft Shadow Mapping (Faster PCSS)

## Signed Distance Field Soft Shadow

This techinque is using on Unreal Engine and is a really state-of-art techinque, so we want to discuss this with more details. 

We actually use a technique called ray marching from the shader point to light point. We can use SDF to know a range around this point without other points. So, we emit a ray to light point, and go along this step by step. A really intuitive way is $\theta$ desribes the visibility, that's the key idea of this methods.

But there are still a lot of problems when we try to implement this model. Like you can see because some details they didn't share to public and we don't have the ability to read source code of UE4. We had tried our best to do it.

* We also need to draw a sphere on light point, only on the path is not enought, otherwise you can see this.

* The generation of Signed Distance Field is really hard. You now just a $64\times 64 \times 64$ pixels SDF is already really large. But if we use some approximation, you can see this.

    Need a image to show the approximation.

* Also, we have a question that why UE won't use vectors to desribe distance? In this way we can interpolate better approximation and also, this method has really huge deviation which means we can decrease some float precision and to achieve same memory storage. We also apply this, and you can see a little promotion(*) .

# End

So, in our group work, we explored the technique about real time rendering, and studied how to render more realistic lighting and shadows. Also, we raised some questions and made a little progress. The code is wrote by OpenGL and we wrote it all by ourselves (but use some third-class libraries), and this project is public on Github if you are interested.

# PBR(Physically Based Rendering)
