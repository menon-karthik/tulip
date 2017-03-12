
This document discussed installation, compilation and the underlying formulation of the Lumped Parameter Network Simulator. 

### Requirements

The following packages should be installed on your local machine before compiling the Lumped Parameter Network Simulator:
- <b>Doxygen</b>: this software is used to build the documentation.
- <b>SWIG</b>: this is use to automatically create the wrapper files to interact with the lumped parameter model via Python. 

### Compilation and installation

The project includes the following files:
- <b>main.cpp</b>: The main function for the c++ code.
- <b>uqTypes.h</b>: Commonly used type declaration. Basically just more compact type defs for std lib vectors and matrices.
- <b>lpnUtils.hpp</b>: Common utilities for LPN models.
- <b>lpnUtils.cpp</b>: Common utilities for LPN models.
- <b>cmModel.h</b>: Main model class.
- <b>cmModel.cpp</b>: Main model class.
- <b>cmLPN_NormalAdult.h</b>: Header file for the LPN model of the circulation in adults.
- <b>cmLPN_NormalAdult.cpp</b>: Source file for the LPN model of the circulation in adults.
- <b>LPN_NormalAdult.i</b>: SWIG Interface file for the LPN Model.

To compile the project type 
~~~
cmake .
~~~
By default, the CMake script in CMakeLists.txt will perform the following operations:

- Compile and link the c++ code into the executable <b>./lpnModel</b>.
- Set up the doxygen documentation files. 
- Use SWIG to build a Python interface for the LPN model.

If you don't want to create a Python interface with SWIG, just use the alternative command:
~~~
cmake -DbuildPythonInterface:BOOL=OFF .
~~~
To build the documentation type:
~~~
make doc
~~~
Both Latex and HTML documentation files will be located in the <b>docs</b> folder.

### Model formulation

This section describes all the components included in the circulation model and associated numerical formulation. 

#### Preliminaries

The circuit layout of the lumped circulation model mimicking the blood circulation in an adult healty patient is illustrated in Figure 1.

<p>
<img src="../imgs/Circuit_FullBody_Normal.jpg" width="80%">
<center><i>Schematic layout of lamped parameter network model for normal adult.</i></center>
</p>

The model includes several compartments:
- <b>Heart model</b>: a bi-ventricular heart model is subdivided into four sub-models simulating the right atrium, right ventricle, left atrium and left ventricle. Each sub-model consists of a pressure-volume relationship governing contraction either in terms of an activation or elastance function. The combination of heart chamber and downstream valve is simulated through an assembly of an inductor, a resistor and a diod allowing blood to flow only in one direction.
- <b>Pulmonary circulation block</b>: The pulmonary circulation is simulated using one capacitor and one resistor. The effects of respiration are not included in the current formulation. 
- <b>Upper body block</b>: Circulation in the upper body is represented through an arterial and venous contribution to the total resistance, while one capacitor is used to lump the overall compliance of the pulmonary circulation. 
- <b>Lower body block</b>: The leg circulation block consists of arterial, capillary and venous resistance, two capacitances for the arteries and veins and a valve.
- <b>Superior Vena Cava</b>: The Superior Vena Cava is simulated using an RC block. 
- <b>Thoracic and Abdominal Inferior Vena Cava</b>: Thoracic and abdominal inferior vena cava are simulated using two RC blocks. No effect of collapsibility in the inferior vena cava is accounted in the current implementation. 
- <b>Thoracic and Abdominal Aorta</b>: These blocks are simulated through RLC equivalent circuits.
- <b>Internal organs blocks</b>: Three internal organ blocks are included in the model describing the liver, kidneys and intestine. All organ blocks are simulated through an LRCR circuit where separated arterial and venous contributions to the total block resistance are separated. 

The varable \f$t\in[0,t_{max}]\f$ denotes the simulation time with maximum equal to \f$t_{max}\f$.
Each heart cycle has a duration of \f$t_c\f$.

#### Atrial and ventricular contraction

Total simulation time is monitored through the variable \f$t\in[0,t_{max}]\f$ while the relative time in the current heart cycle can be determing from the quantity \f$t_{cr} = t fmod t_c\f$ where \f$fmod\f$ denotes the floating point reminder operation and each heart cycle has a duration equal to \f$t_c\f$.

Atrial contraction is simulated through an activation model characterized by the contraction duration and time shift, respectively. 
The relative duration of the atrial pulse in the heart cycle is modeled using $t_{sa,s}$, while the absolute atrial contraction time is obtained from \f$t_{sa} = t_c\,t_{sa,s}\f$.
The instant in time when the atrial pulse starts is identified through the parameter \f$t_{pw}\f$. 
Atrial contraction will start near the end of the cardiac cycle at \f$t = t_c - (t_c/t_{p,w})\f$.

Atrial activation is defined in time using the following expression:

\f{eqnarray*}{
\begin{cases}
f_{AA} = \frac{1}{2}\,\left[1 - \cos\left(\frac{2\pi\,(t_{cr}-t_{pw}+t_{sa})}{t_{sa}}\right)\right] & \mbox{if}\quad  t_{cr} \le t_{pw}, \\[1em]
f_{AA} = \frac{1}{2}\,\left[1 - \cos\left(\frac{2\pi\,[t_{cr}-t_{pw}-(t_c-t_{sa})]}{t_{sa}}\right)\right]  & \mbox{if}\quad t_{cr}\ge(t_c-t_{sa})+t_{pw}\quad \mbox{and}\quad t_{c,r}<t_c,\\[1em]
f_{AA} = 0 & \mbox{otherwise}.\\
\end{cases}
\f}

Passive atrial curves are also defined as

\f{eqnarray*}{
\begin{cases}
P_{pas,ra} & = \lambda_{1,ra}\left[\,e^{\, \lambda_{2,ra}\,(V_{ra}-V_{ra,0})}-1\right],\\[1em]
P_{pas,la} & =  \lambda_{1,la}\left[\,e^{\, \lambda_{2,la}\,(V_{la}-V_{la,0})}-1\right],
\end{cases}
\f}

where the coefficients \f$\lambda_{1}\f$, \f$\lambda_{2}\f$ govern the slope and exponential increase of the passive curve, respectively and \f$V_{ra,0}\f$, \f$V_{la,0}\f$ denote the unstressed volumes in the right and left atria. 
The active atrial curve is linear with constant slope of \f$E_{max,ra}\f$ and \f$E_{max,la}\f$ for the right and left atrium, respectively, according to the following expression:

\f{eqnarray*}{
\begin{cases}
P_{act,ra} & = E_{max,ra}\,(V_{ra}-V_{ra,0})\\[1em]
P_{act,la} & =  E_{max,la}\,(V_{la}-V_{la,0})\\
\end{cases}
\f}

Pressure in the atria is bounded by the passive and active pressure, modulated by the activation function:

\f{eqnarray*}{
\begin{cases}
P_{ra} = P_{pas,ra} + f_{AA}\,(P_{act,ra}-P_{pas,ra}),\\[1em]
P_{la} = P_{pas,la} + f_{AA}\,(P_{act,la}-P_{pas,la}).\\
\end{cases}
\f}

The mechanism of ventricular contraction is instead modeled using an elastance approach. 
We defined the quantities \f$E_{rv,s}\f$, \f$E_{lv,s}\f$ as the elastance scaling parameter for the right and left ventricle, respectively.

The elastance function and its time derivative is computed from [2] using the parameters \f$E_{Max} = 2.31\f$, \f$E_{Min} = 0.06\f$, \f$a_1 = 0.303\f$, \f$a_2 = 0.508\f$, \f$n_1 = 1.32\f$, \f$n_2 = 21.9\f$ and the following expression:

\f[
E(t) = E_{Min} + E_{Max}\,\left[\frac{\left(\frac{t_r}{a_1}\right)^{n_1}}{1 + \left(\frac{t_r}{a_1}\right)^{n_1}} \cdot \frac{1}{1 + \left(\frac{t_r}{a_2}\right)^{n_2}}\right]
\f]

Elastance functions for the left and right atrium, respectively, are defined as:

\f{eqnarray*}{
\begin{cases}
E_{rv}(t) = E_{rv,s}\cdot E(t),\\[1em]
E_{lv}(t) = E_{rv,s}\cdot E(t),\\[1em]
\end{cases}
\f}

Finally, the ventricular pressures are determined using the expressions:

\f{eqnarray*}{
\begin{cases}
P_{lv} = E_{lv}\,(V_{lv} - V_{lv,0}),\\[1em]
P_{rv} = E_{rv}\,(V_{lv} - V_{rv,0}).\\
\end{cases}
\f}

with \f$V_{lv,0}\f$, \f$V_{rv,0}\f$ the left and right unstressed ventricular volumes, respectively.

#### Heart model components including lung circulation

The equations for the heart model components (i.e., right atrium, right ventricle, left atrium and left ventricle) are assembled from a series arrangement of inductor, diod and resistor.
We start by defining an Heaviside function that governs the opening and closing of heart valves, i.e.

\f{eqnarray*}{
\phi_{i,j} = H(P_i-P_j) = 
\begin{cases}
1.0\quad\mbox{if}\quad P_i > P_j\\[1em]
0.0\quad\mbox{if}\quad P_i < P_j.
\end{cases}
\f}

The differential equation describing evolution of flow between right atrium and right ventricle, assumes the form:

\f[
\frac{dQ_{ra,rv}}{dt} = \phi_{ra,rv}\cdot\frac{1}{L_{ra,rv}}\cdot\left(P_{ra} - P_{rv} - R_{ra,rv}\cdot Q_{ra,rv}\right)
\f]

A similar equation can be written for the flow between the right ventricle and the lungs:

\f[
\frac{dQ_{rv,pa}}{dt} =  \phi_{rv,pa}\cdot\frac{1}{L_{rv,pa}}\cdot\left(P_{rv} - P_{pa} - R_{rv,pa}\cdot Q_{rv,pa}\right),
\f]

and the flow in the lungs can be represented as:

\f{eqnarray*}{
\begin{cases}
Q_{pul} &= \frac{P_{pa}-P_{la}}{R_{pul}}\\[1em]
\frac{dP_{pa}}{dt} &= \frac{1}{C_{pul}} \cdot \left(Q_{rv,pa}\cdot\phi_{rv,pa} - Q_{pul}\right).
\end{cases}
\f}

Similarly, the flow between the left atrium and left ventricle is simulated using the following expression: 

\f[
\frac{dQ_{la,lv}}{dt} = \phi_{la,lv}\cdot\frac{1}{L_{la}}\cdot\left(P_{la}-P_{lv} - R_{la,lv}\cdot Q_{la,lv}\right).
\f]

Finally the flow equation for the flow exiting from the left ventricle and entering the aorta becomes:

\f[
\frac{dQ_{lv,ao}}{dt} = \phi_{rv,pa}\cdot\frac{1}{L_{lv,ao}}\cdot (P_{lv}-P_{ao}-R_{lv,ao}\cdot Q_{lv,ao}).
\f]

With reference to Figure 1, in the above equations we denoted by \f$L_{ra,rv}\f$, \f$R_{ra,rv}\f$ the inductance and resistance in the right atrium, \f$L_{rv,pa}\f$, \f$R_{rv,pa}\f$ those in the right ventricle. Similarly, \f$L_{la,lv}\f$, \f$R_{la,lv}\f$ denote the inductance and resistance in the left atrium while \f$L_{lv,ao}\f$, \f$R_{lv,ao}\f$ are the same quantities for the left ventricle.
The following differential equations complete the heart model by relating the time derivative of the volume in the four chambers to their net flow rate.

\f{eqnarray*}{
\begin{cases}
\frac{dV_{ra}}{dt} &= Q_{svc} + Q_{thivc} - Q_{ra,rv} \cdot \phi_{ra,rv}\\[1em]
\frac{dV_{rv}}{dt} &= Q_{ra,rv} \cdot \phi_{ra,rv} - Q_{rv,pa} \cdot \phi_{rv,pa}\\[1em]
\frac{dV_{la}}{dt} &= Q_{pul} - Q_{la,lv} \cdot \phi_{la,lv}\\[1em]
\frac{dV_{lv}}{dt} &= Q_{la,lv} \cdot \phi_{la,lv} - Q_{lv,ao} \cdot \phi_{lv,ao}
\end{cases}
\f}

#### Aortic arch capacitance

The capacitance of the aortic arch is considered equal to \f$C_{ao}\f$ and the following equation governs the change in pressure at that location:

\f[
\frac{dP_{ao}}{dt} = \frac{1}{C_{ao}}\cdot \left( Q_{lv,ao} - Q_{uba} - Q_{thao} \right)
\f]

#### Upper Body Circulation

\f{eqnarray*}{
\begin{cases}
\frac{dQ_{uba}}{dt} = \frac{1}{L_{uba}}\cdot\left(P_{ao} - R_{uba}\cdot Q_{uba} - P_{ub} \right),\\[1em]
Q_{ubv} = \frac{P_{ub}-P_{svc}}{R_{ubv}},\\[1em]
\frac{dP_{ub}}{dt} = \frac{1}{C_{ub}} \cdot \left(Q_{uba} - Q_{ubv}\right).
\end{cases}
\f}

#### Superior Vena Cava

\f{eqnarray*}{
\begin{cases}
Q_{svc} = \frac{P_{svc}-P_{ra}}{R_{svc}},\\[1em]
\frac{dP_{svc}}{dt} = \frac{1}{C_{svc}} \cdot \left(Q_{ubv} - Q_{svc}\right).
\end{cases}
\f}

#### Thoracic Aorta

\f{eqnarray*}{
\begin{cases}
\frac{dQ_{thao}}{dt} = \frac{1}{L_{thao}}\cdot\left(P_{ao} - R_{thao}\cdot Q_{thao} - P_{thao} \right),\\[1em]
Q_{abao} = \frac{ P_{thao} - P_{abao} }{ R_{abao} },\\[1em]
\frac{dP_{thao}}{dt} = \frac{1}{C_{thao}} \cdot \left( Q_{thao} - Q_{abao} - Q_{la} - Q_{ka} \right).
\end{cases}
\f}

#### Abdominal Aorta

\f{eqnarray*}{
\begin{cases}
\frac{dQ_{abao}}{dt} = \frac{1}{L_{abao}}\cdot\left(P_{thao} - R_{abao}\cdot Q_{abao} - P_{abao} \right),\\[1em]
Q_{lega} = \frac{P_{abao}-P_{lega}}{R_{lega}},\\[1em]
Q_{ia} = \frac{P_{abao}-P_{i}}{R_{ia}},\\[1em]
\frac{dP_{abao}}{dt} = \frac{1}{C_{abao}} \cdot \left( Q_{abao} - Q_{ia} - Q_{lega} \right).
\end{cases}
\f}

#### Lower Body Circulation

\f{eqnarray*}{
\begin{cases}
\frac{dQ_{lega}}{dt} = \frac{1}{L_{lega}}\cdot\left(P_{abao} - R_{lega}\cdot Q_{lega} - P_{lega} \right),\\[1em]
Q_{legc} = \frac{P_{lega}-P_{legv}}{R_{legc}},\\[1em]
\frac{dP_{lega}}{dt} = \frac{1}{C_{lega}} \cdot \left( Q_{lega} - Q_{legc} \right),\\[1em]
Q_{legv} = \frac{P_{legv}-P_{abivc}}{R_{legv}},\\[1em]
\frac{dP_{legv}}{dt} = \frac{1}{C_{legv}} \cdot \left( Q_{legc} - \cdot \phi_{legv,abivc} \right).
\end{cases}
\f}

#### Abdominal IVC

\f{eqnarray*}{
\begin{cases}
Q_{abivc} = \frac{P_{abivc}-P_{thivc}}{R_{abivc}},\\[1em]
\frac{dP_{abivc}}{dt} = \frac{1}{C_{abivc}} \cdot \left( Q_{legv}\cdot\phi_{legv,abivc} - Q_{abivc} \right).
\end{cases}
\f}

#### Thoracic IVC

\f{eqnarray*}{
\begin{cases}
Q_{thivc} = \frac{P_{thivc}-P_{ra}}{R_{thivc}},\\[1em]
\frac{dP_{thivc}}{dt} = \frac{1}{C_{thivc}} \cdot \left( Q_{abivc} - Q_{thivc} \right).
\end{cases}
\f}

#### Liver Circulation

\f{eqnarray*}{
\begin{cases}
\frac{dQ_{lla}}{dt} = \frac{1}{L_{lla}}\cdot\left( P_{thao} - R_{lla}\cdot Q_{lla} - P_{ll} \right),\\[1em]
Q_{iv} = \frac{P_{i}-P_{ll}}{R_{iv}},\\[1em]
Q_{llv} = \frac{P_{ll}-P_{thivc}}{R_{llv}},\\[1em]
\frac{dP_{ll}}{dt} = \frac{1}{C_{ll}} \cdot \left(Q_{lla} + Q_{iv} - Q_{llv} \right).
\end{cases}
\f}

#### Circulation in the kidneys

\f{eqnarray*}{
\begin{cases}
\frac{dQ_{ka}}{dt} = \frac{1}{L_{ka}}\cdot\left( P_{thao} - R_{ka}\cdot Q_{ka} - P_{k} \right),\\[1em]
Q_{kv} = \frac{P_{k}-P_{thivc}}{R_{kv}},\\[1em]
\frac{dP_{k}}{dt} = \frac{1}{C_{k}} \cdot \left( Q_{ka} - Q_{kv} \right).
\end{cases}
\f}

#### Circulation in the intestine

\f{eqnarray*}{
\begin{cases}
\frac{dQ_{ia}}{dt} = \frac{1}{L_{ia}}\cdot\left(P_{abao} - R_{ia}\cdot Q_{ia} - P_{i} \right),\\[1em]
Q_{iv} = \frac{P_{i}-P_{ll}}{R_{iv}},\\[1em]
\frac{dP_{i}}{dt} = \frac{1}{C_{i}} \cdot \left(Q_{ia} - Q_{iv}\right).
\end{cases}
\f}

### Default model parameters

The following table lists the initial conditions adopted for the state variables

Variable | Unit | Initial condition
---------| ---- | -----------------
  \f$V_{ra}\f$ | ml | 0.0 
  \f$V_{la}\f$ | ml | 0.0 
  \f$V_{rv}\f$ | ml | 0.0 
  \f$V_{lv}\f$ | ml | 0.0
  \f$Q_{ra,rv}\f$ | ml/s | 0.0
  \f$P_{pa}\f$ | mmHg | 70.0
  \f$Q_{rv,pa}\f$ | ml/s | 0.0
  \f$Q_{la,lv}\f$ | ml/s | 0.0
  \f$P_{ao}\f$ | mmHg | 100.0
  \f$Q_{lv,ao}\f$ | ml/s | 0.0
  \f$P_{ub}\f$ | mmHg | 50.0
  \f$Q_{uba}\f$ | ml/s | 0.0
  \f$P_{svc}\f$ | mmHg | 10.0
  \f$Q_{thao}\f$ | ml/s | 0.0
  \f$P_{thao}\f$ | mmHg | 120.0
  \f$P_{abao}\f$ | mmHg | 120.0
  \f$Q_{abao}\f$ | ml/s | 0.0
  \f$P_{i}\f$ | mmHg | 50.0
  \f$P_{lega}\f$ | mmHg | 50.0
  \f$P_{abivc}\f$ | mmHg | 50.0
  \f$P_{legv}\f$ | mmHg | 50.0
  \f$Q_{lega}\f$ | ml/s | 0.0
  \f$P_{thivc}\f$ | mmHg | 10.0
  \f$Q_{lla}\f$ | ml/s | 0.0
  \f$P_{ll}\f$ | mmHg | 120.0
  \f$Q_{ka}\f$ | ml/s | 0.0
  \f$P_{k}\f$ | mmHg | 120.0
  \f$Q_{ia}\f$ | ml/s | 0.0
  
Atrial model parameters and heart rate

Variable | Unit | Initial condition
---------| ---- | -----------------
HR - Heart Rate | bpm | 78.0
\f$t_{sa}\f$ - Atrial relative activation duration | - | 0.4
\f$t_{pw}\f$ - Atrial relative activation time shift | - | 9.5 
\f$\lambda_{1,ra}\f$ - Atrial passive curve slope, right atrium | - | 4.0
\f$\lambda_{2,ra}\f$ - Atrial passive curve exponent factor, right atrium | - | 0.006 
\f$E_{max,ra}\f$ - Atrial active curve slope, right atrium | Barye/ml | 0.1 
\f$V_{ra,0}\f$ - Unstressed right atrial volume | ml | 0.0 
\f$\lambda_{1,la}\f$ - Atrial passive curve slope, left atrium | - | 8.0 
\f$\lambda_{2,la}\f$ - Atrial passive curve exponent factor, left atrium | - | 0.0065 
\f$E_{max,la}\f$ - Atrial active curve slope, left atrium | Barye/ml | 0.5
\f$V_{la,0}\f$ - Unstressed left atrial volume | ml | 0.0 

Ventricular model parameters

Variable | Unit | Initial condition
---------| ---- | -----------------
\f$E_{rv,s}\f$ - Right Ventricular Elastance Scaling Factor | - | 0.6
\f$E_{lv,s}\f$ - Left Ventricular Elastance Scaling Factor | - | 1.04
\f$E_{lvp,s}\f$ - Left Ventricular Elastance Derivative Scaling Factor | - | 6.0 
\f$V_{rv,0}\f$ - Unstressed right ventricular volume | ml | 0.0 
\f$V_{lv,0}\f$ - Unstressed left ventricular volume | ml | 0.0 
  
Atrial and Ventricular Inductances and Resistances

Variable | Unit | Initial condition
---------| ---- | -----------------
\f$L_{ra,rv}\f$ - Inductance of right atrium | Barye s\f$^2\f$/ml | 0.1 
\f$R_{ra,rv}\f$ - Resistance of right atrium | Barye s/ml | 20.0 
\f$L_{rv,pa}\f$ - Inductance of right ventricle | Barye s\f$^2\f$/ml | 0.1 
\f$R_{rv,pa}\f$ - Resistance of right ventricle | Barye s/ml | 30.0 
\f$L_{la,lv}\f$ - Inductance of left atrium | Barye s\f$^2\f$/ml | 0.1 
\f$R_{la,lv}\f$ - Resistance of left atrium | Barye s/ml | 50.0 
\f$L_{lv,ao}\f$ - Inductance of left ventricle | Barye s\f$^2\f$/ml | 0.1 
\f$R_{lv,ao}\f$ - Resistance of left ventricle | Barye s/ml | 20.0 

Peripheral model properties

Variable | Unit | Initial condition
---------| ---- | -----------------
<b>Pulmonary Circulation</b> | - | -
\f$C_{pa}\f$ - Pulmonary capacitance | mmHg/ml | 5.0
\f$R_{pa}\f$ - Pulmonary resistance | Barye s/ml | 50.0 
<b>Aortic Arch</b> | - | -
\f$C_{ao}\f$ - Aortic capacitance  | mmHg/ml | 0.5
<b>Upped Body Circulation Parameters</b> | - | -
\f$L_{uba}\f$ - Upper body arterial inductance | Barye s\f$^2\f$/ml | 0.1 
\f$R_{uba}\f$ - Upper body arterial resistance | Barye s/ml | 200.0 
\f$C_{ub}\f$ - Upper body compliance | Barye/ml | 100.0e-6 
\f$R_{ubv}\f$ - Upper body venous resistance | Barye s/ml | 700.0 
<b>Superior Vena Cava</b> | - | -
\f$C_{svc}\f$ - Compliance of SVC | Barye/ml | 400.0e-6 
\f$R_{svc}\f$ - SVC Resistance | Barye s/ml | 50.0 
<b>Thoracic Aorta</b> | - | -
\f$L_{thao}\f$ - Inductance of thoracic aorta | Barye s\f$^2\f$/ml | 1.0 
\f$R_{thao}\f$ - Resistance of thoracic aorta | Barye s/ml | 15.0 
\f$C_{thao}\f$ - Capacitance of thoracic aorta | Barye/ml | 300.0e-6; 
<b>Abdominal Aorta</b> | - | -
\f$R_{abao}\f$ - Resistance of abdominal aorta | Barye s/ml | 50.0 
\f$L_{abao}\f$ - Inductance of abdominal aorta | Barye s\f$^2\f$/ml | 1.0 
\f$C_{abao}\f$ - Capacitance of abdominal aorta | Barye/ml | 100.0e-6
<b>Legs</b> | - | -
\f$L_{lega}\f$ - Inductance of leg arteries | Barye s\f$^2\f$/ml | 0.1 
\f$R_{lega}\f$ - Resistance of leg arteries | Barye s/ml | 100.0
\f$C_{lega}\f$ - Capacitance of leg arteries | Barye/ml | 450.0e-6 
\f$R_{legc}\f$ - Resistance of leg capillary circulation | Barye s/ml | 1000.0 
\f$C_{legv}\f$ - Capacitance of leg venous circulation | Barye/ml | 6000.0e-6 
\f$R_{legv}\f$ - Resistance of leg venous circulation | Barye s/ml | 500.0 
<b>Abdominal IVC - Inferior Vena Cava</b> | - | -
\f$C_{abivc}\f$ - Capacitance of abdominal IVC | Barye/ml | 500.0e-6 
\f$R_{abivc}\f$ - Resistance of abdominal IVC | Barye s/ml | 200.0 
<b>Thoracic IVC</b> | - | -
\f$C_{thivc}\f$ - Capacitance of thoracic IVC | Barye/ml | 500.0e-6 
\f$R_{thivc}\f$ - Resistance of thoracic IVC | Barye s/ml | 700.0
<b>Liver</b> | - | -
\f$L_{lla}\f$ - Inductance of Liver arterial circulation | Barye s\f$^2\f$/ml | 10.0 
\f$R_{lla}\f$ - Resistance of Liver arterial circulation | Barye s/ml | 200.0 
\f$C_{ll}\f$ - Capacitance of Liver circulation | Barye/ml | 1890.0e-6 
\f$R_{llv}\f$ - Resistance of Liver venous circulation | Barye s/ml | 2000.0 
<b>Kidneys</b> | - | -
\f$L_{ka}\f$ - Inductance of arterial circulation in the Kidneys | Barye s\f$^2\f$/ml | 10.0 
\f$R_{ka}\f$ - Resistance of arterial circulation in the Kidneys | Barye s/ml | 200.0
\f$C_{k}\f$ - Capacitance of circulation in the Kidneys | Barye/ml | 7200.0e-6 
\f$R_{kv}\f$ - Resistance of venous circulation in the Kidneys | Barye s/ml | 2000.0 
<b>Intestine</b> | - | -
\f$L_{ia}\f$ - Inductance of arterial circulation in the Intestine | Barye s\f$^2\f$/ml | 10.0 
\f$R_{ia}\f$ - Resistance of arterial circulation in the Intestine | Barye s/ml | 200.0 
\f$C_{i}\f$ - Capacitance of circulation in the Intestine | Barye/ml | 1130.0e-6 
\f$R_{iv}\f$ - Resistance of venous circulation in the Intestine | Barye s/ml | 2000.0 

### Model solution with default parameter set

<p>
<img src="../imgs/HeartChambers.jpg" width="50%">
<center><i>Atrial and ventricular activation shift (top left). Pressures in the right atrium, right ventricle and lung circulation (top left). Pressures in the left atrium, left ventricle and aorta (bottom left). Atrial and ventricular volumes (bottom right).</i></center>
</p>

<p>
<img src="../imgs/PV-loops.jpg" width="50%">
<center><i>Atrial and ventricular Pressure-Volume loops.</i></center>
</p>

<p>
<img src="../imgs/HeartChamberFlow.jpg" width="50%">
<center><i>Flow in the heart chambers.</i></center>
</p>

<p>
<img src="../imgs/PeripheralFlow.jpg" width="50%">
<center><i>Flow rates in peripheral circulation.</i></center>
</p>

<p>
<img src="../imgs/PeripheralPressure.jpg" width="50%">
<center><i>Pressures in peripheral circulation.</i></center>
</p>

### Bibliography

[1] Snyder M.F. and Rideout V.C., <b>Computer simulation studies of the venous circulation</b>, <i>Biomedical Engineering, IEEE Transactions on</i>, 4, 325-334, 1969.

[2] Stergiopulos N., Meister, J.J. and Westerhof N., <b>Determinants of stroke volume and systolic and diastolic aortic pressure</b>, <i>American Journal of Physiology-Heart and Circulatory Physiology</i>, 270(6), H2050-H2059, 1996.

