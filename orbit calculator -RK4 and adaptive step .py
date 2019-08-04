import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation


class Particle:

    def __init__(self, mass):
        self.R = np.zeros((int(N), 3))
        self.V = np.zeros((int(N), 3))
        self.M = mass
        self.time = np.zeros((int(N)))

    def initialstate(self, x, y, z, vx, vy, vz):
        self.R[0, 0] = x
        self.R[0, 1] = y
        self.R[0, 2] = z
        self.V[0, 0] = vx
        self.V[0, 1] = vy
        self.V[0, 2] = vz
        self.time[0] = 0


class Gravity:
    def __init__(self, centres, orbits):
        self.centres = centres
        self.orbs = orbits
    ''' Function defined to compute the acceleration and velocity due to the gravitational interaction between a test-particle in the presence of a massive object 'gravitating_object'
    using a Runge-Kutta fourth order method. The function returns two coefficients, KTv abd KTp which can then be used to propagate the velocity and position a step forward
    dT in time by multiplying theses coefficients by dT.  '''

    def RK4_acceleration(self, gravitating_object, testparticle, h, i):

        dis = ((testparticle.R[i, 0] - gravitating_object.R[i, 0])**2 + (testparticle.R[i, 1] - gravitating_object.R[i, 1])**2 + (testparticle.R[i, 2] - gravitating_object.R[i, 2])**2)**0.5
        K1v = np.zeros(3)
        K1p = np.zeros(3)
        K2v = np.zeros(3)
        K2p = np.zeros(3)
        K3v = np.zeros(3)
        K3p = np.zeros(3)
        K4v = np.zeros(3)
        K4p = np.zeros(3)
        KTv = np.zeros(3)
        KTp = np.zeros(3)
        a = np.zeros(3)
        if gravitating_object.M != 0:
            for k in range(2):
                a[k] = - gravitating_object.M * G * testparticle.R[i, k] / dis**3
                K1v[k] = a[k] * h
                K1p[k] = testparticle.V[i, k] * h

            dis2 = ((testparticle.R[i, 0] - gravitating_object.R[i, 0] + K1p[0] * h / 2)**2 + (testparticle.R[i, 1] - gravitating_object.R[i, 1] + K1p[1] * h / 2)**2 + (testparticle.R[i, 2] - gravitating_object.R[i, 2] + K1p[2] * h / 2)**2)**0.5
            for k in range(2):
                K2v[k] = -(gravitating_object.M * G * (testparticle.R[i, k] + (K1p[k] * h / 2))) / dis2**3
                K2p[k] = testparticle.V[i, k] + (K1v[k] * h / 2)

            dis3 = ((testparticle.R[i, 0] - gravitating_object.R[i, 0] + (K2p[0] * h / 2))**2 + (testparticle.R[i, 1] - gravitating_object.R[i, 1] + (K2p[1] * h / 2))**2 + (testparticle.R[i, 2] - gravitating_object.R[i, 2] + (K2p[2] * h / 2))**2)**0.5
            for k in range(2):
                K3v[k] = -(gravitating_object.M * G * (testparticle.R[i, k] + (K2p[k] * h / 2))) / dis3**3
                K3p[k] = testparticle.V[i, k] + (K2v[k] * h / 2)

            dis4 = ((testparticle.R[i, 0] - gravitating_object.R[i, 0] + (K3p[0] * h))**2 + (testparticle.R[i, 1] - gravitating_object.R[i, 1] + (K3p[1] * h))**2 + (testparticle.R[i, 2] - gravitating_object.R[i, 2] + (K3p[2] * h))**2)**0.5
            for k in range(2):
                K4v[k] = -(gravitating_object.M * G * (testparticle.R[i, k] + (K3p[k] * h))) / dis4**3
                K4p[k] = testparticle.V[i, k] + (K3v[k] * h)

            KTv = ((1 / 6) * (K1v + (2 * K2v) + (2 * K3v) + K4v))
            KTp = ((1 / 6) * (K1p + (2 * K2p) + (2 * K3p) + K4p))
        return KTv, KTp

    ''' Function defined to return two coefficients to propagte the velocity and position using a first order Euler method. '''

    def Euler_acceleration(self, gravitating_object, testparticle, h, i):
        dis = ((testparticle.R[i, 0] - gravitating_object.R[i, 0])**2 + (testparticle.R[i, 1] - gravitating_object.R[i, 1])**2 + (testparticle.R[i, 2] - gravitating_object.R[i, 2])**2)**0.5
        K1v = np.zeros(3)
        K1p = np.zeros(3)
        a = np.zeros(3)
        for k in range(3):
            a[k] = - gravitating_object.M * G * testparticle.R[i, k] / dis**3
            K1v[k] = a[k]
            K1p[k] = testparticle.V[i, k]
        return K1v, K1p

    ''' Function defined to propagate the velocity and position of massive objects defined by the array 'objects' which are all massive enough so that they all produce a
    significant gravitational influence on each other. '''

    def iterate_mutualattract_RK4(self, test_particle, target_index, h, i):
        positioniterator = np.zeros(3)
        velocityiterator = np.zeros(3)
        iteratorv = np.zeros((len(centres), 3))
        iteratorx = np.zeros((len(centres), 3))
        for index in range(len(self.centres)):
            if index != target_index:
                velocityiterator, positioniterator = self.RK4_acceleration(self.centres[index], test_particle, h, i)
                iteratorv[index, :] = velocityiterator
                iteratorx[index, :] = positioniterator
        test_particle.V[i + 1, :] = h * np.sum(iteratorv, axis=0) + test_particle.V[i, :]
        test_particle.R[i + 1, :] = h * np.sum(iteratorx, axis=0) + test_particle.R[i, :]
        return test_particle.V[i + 1, :], test_particle.R[i + 1, :]

    ''' Function defined which propagates the velocity and position of a set of test-particles which have small masses so that they do not produce a significant
    gravitational influence on one another. Because of this, to propagate their coordinates only the gravitational influence of the objects in the array 'orbits' is
    accounted for in the calculation of the KTv and KTp coefficients. '''

    def iterate_centralfield_RK4(self, testparticle, h, i):
        positioniterator = np.zeros(3)
        velocityiterator = np.zeros(3)
        iteratorv = np.zeros((len(centres), 3))
        iteratorx = np.zeros((len(centres), 3))
        for j in range(len(centres)):
            velocityiterator, positioniterator = self.RK4_acceleration(self.centres[j], testparticle, h, i)
            iteratorv[j, :] = velocityiterator
            iteratorx[j, :] = positioniterator
        testparticle.V[i + 1, :] = h * np.sum(iteratorv, axis=0) + testparticle.V[i, :]
        testparticle.R[i + 1, :] = h * iteratorx[0] + testparticle.R[i, :]
        return testparticle.V[i + 1, :], testparticle.R[i + 1, :]


if __name__ == '__main__':
    ''' The total time for which the simulation is to be run is defined in addition to the base time interval dT. The adapted time, 'dT_adapted'
    is used to propagate the velocity and position of the bodies in the simulation, and it is a fraction of dT determined by the adaptive step code.
    The physical constant G is defined also, in addition to the masses of the bodies in the simulation: the mass of the massive bodies is taken as
    10^12, as the program is currently set-up to simmulate galaxy interaction, and the mass of a galaxy is approximately 12 orders of magnitude
    greater than a star. The letters R and r hold the distances used in computations, in units of kiloparsecs, and thus the time is in units of
    500000 years (the same units as used in Toomre & Toomre 'Galactic Bridges and tails' 1972, The Astrophysical journal, 178, pg 623-666).  '''
    T = 2000
    dT = 0.9
    dT_adapted = dT
    time = 0
    N = 1000000
    G = 1e-11
    M = 1e12
    m = 1
    r = 25  # in kiloparecs
    R = 10 * r
    V = (G * M / R)**0.5
    v = (G * M / r)**0.5
    centre1 = Particle(M)
    centre1.initialstate(0, 0, 0, 0, 0, 0)
    centre2 = Particle(30 * M)
    centre2.initialstate(-0.5 * R, R, 0, 0.35 * V, -2 * V, 0)

    centres = [centre1, centre2]

    ''' Code which initialises a set of test particles with masses between 0.2 and 1.2 solar masses, in circular orbits between 0.4 and 1.4 times r, around a
    central galaxy mass at (0,0,0).  '''
    orbits = []
    difference_centres = []
    for i in range(10):
        new_particle = Particle((np.random.rand() + 0.2) * m)
        rx = (2 * np.random.rand() - 1) * r
        ry = (2 * np.random.rand() - 1) * r
        r0 = (rx**2 + ry**2)**0.5
        vy = (G * M / r0)**0.5 * rx / r0
        vx = -(G * M / r0)**0.5 * ry / r0
        new_particle.initialstate(rx, ry, 0, vx, vy, 0)
        if (rx**2 + ry**2)**0.5 >= 0.3 * r:
            orbits.append(new_particle)
    gravity = Gravity(centres, orbits)
    i = 0

    ''' Loop defined to simulate the propagation of the test-particles and galaxy centres defined above. An adaptive step size algorithm is implemented
    to make the simulation more efficient.  '''
    while time < T:
        solution1 = []
        solution2 = []
        difference = []
        vector = np.zeros(3)
        ''' adaptive step size algorithm to optimise run time and accuracy of computation.'''
        for k in range(len(centres)):  # Iteration with initial step size.
            solution1.append(gravity.iterate_mutualattract_RK4(centres[k], k, dT, i))
        for k in range(len(orbits)):
            solution1.append(gravity.iterate_centralfield_RK4(orbits[k], dT, i))

        for k in range(len(centres)):  # Iteration with half initial step size to time + dT/2.
            centres[k].V[i + 1, :], centres[k].R[i + 1, :] = gravity.iterate_mutualattract_RK4(centres[k], k, dT / 2, i)
        for k in range(len(orbits)):
            orbits[k].V[i + 1, :], orbits[k].R[i + 1, :] = gravity.iterate_centralfield_RK4(orbits[k], dT / 2, i)

        for k in range(len(centres)):  # Iterate with half step size to get to time + dT.
            solution2.append(gravity.iterate_mutualattract_RK4(centres[k], k, dT / 2, i + 1))
        for k in range(len(orbits)):
            solution2.append(gravity.iterate_centralfield_RK4(orbits[k], dT / 2, i + 1))

        for k in range(len(solution1)):  # Difference in vectors when [i + 1] is determined with dT and 2 * dT/2.
            difference.append(np.linalg.norm(solution1[k]) - np.linalg.norm(solution2[k]))

        if np.linalg.norm(difference) != 0:  # Adjustment of step size so that the error is below a given bound.
            dT_adapted = dT * (0.008008522 / np.linalg.norm(difference))**0.2
        time += dT_adapted

        for k in range(len(centres)):  # Determination of i + 1 using the adatped step size.
            centres[k].V[i + 1, :], centres[k].R[i + 1, :] = gravity.iterate_mutualattract_RK4(centres[k], k, dT_adapted, i)
            centres[k].time[i + 1] = time
        for k in range(len(orbits)):
            orbits[k].V[i + 1, :], orbits[k].R[i + 1, :] = gravity.iterate_centralfield_RK4(orbits[k], dT_adapted, i)
            orbits[k].time[i + 1] = time
        vector = centres[0].R[i, :] - centres[1].R[i, :]
        a = np.linalg.norm(vector)
        difference_centres.append(a)
        i += 1


    # Analysis of motion of bodies.
    plotting_lim = i
    print(time, i, 'average step size=', time / i)
    closest_approach = np.amin(difference_centres)
    print('closest approach distance=', closest_approach / r)
    print('number of orbits:', print(len(orbits)))
    actual_time = ((centres[1].R[0, 0] - centres[1].R[plotting_lim, 0]) * 3.089 * 1e19) / (131069) # Scaling of times and distances.
    print('time passed=', actual_time)
    # energy, for analysis of accuracy of circular orbits:
    # fchange = []
    # for i in range(len(orbits)):
    #     U0 = (0.5 * G * centres[0].M * orbits[0].M) / (np.linalg.norm(orbits[0].R[0]))
    #     Uf = (0.5 * G * centres[0].M * orbits[0].M) / (np.linalg.norm(orbits[0].R[plotting_lim]))
    #     fractionalchange = (Uf - U0) / U0
    #     fchange.append(fractionalchange)
    # print('fractional change in energy=', np.mean(fchange))

    # Plot of orbit paths.
    fig1 = plt.figure()
    ax = fig1.add_subplot(111)
    for i in range(len(orbits)):
        if np.linalg.norm(orbits[i].R[plotting_lim, :]) <= 60000:
            plt.plot(orbits[i].R[0: plotting_lim, 0], orbits[i].R[0: plotting_lim, 1], linewidth=0.01)
    plt.plot(centres[0].R[0: plotting_lim, 0], centres[0].R[0: plotting_lim, 1], 'b', linewidth=1)
    plt.plot(centres[1].R[0:plotting_lim, 0], centres[1].R[0: plotting_lim, 1], 'm', linewidth=0.01)
    #plt.plot(centres[2].R[0:plotting_lim, 0], centres[2].R[0: plotting_lim, 1], 'm', linewidth=0.01)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    plt.tight_layout()
    plt.savefig('orbitsXYZ.pdf', bbox="tight")


    data1 = centres[0].R
    data2 = centres[1].R
    xorbitdata = np.zeros(len(orbits)+2)
    yorbitdata = np.zeros(len(orbits)+2)
    fig, ax = plt.subplots()
    ax.set(xlim=(- 2*max(data2[:,0]),  2*max(data2[:,0])), ylim=(- 2*max(data2[:,1]),  2*max(data2[:,1])))
    scat = plt.scatter([], [], s=1,)

    # Animate the particles' motion in plane.
    def animate(u):
        for i in range(len(orbits)-2):
            xorbitdata[i] = orbits[i].R[u,0]
            yorbitdata[i] = orbits[i].R[u,1]
        xorbitdata[len(orbits)-1] = data1[u,0]
        xorbitdata[len(orbits)] = data2[u,0]
        yorbitdata[len(orbits)-1] = data1[u,1]
        yorbitdata[len(orbits)] = data2[u,1]

        data = np.vstack((xorbitdata, yorbitdata))
        dataB = data.T

        scat.set_offsets(data.T)
        return scat,

    anim = animation.FuncAnimation(fig, animate ,frames=plotting_lim, interval=1000, blit=True)
    anim.save('displacement.mp4', fps=200)


    # Plots of the shape of central galaxy at different instants in time during the simmulation.
    unplotted = []
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.scatter(centre1.R[plotting_lim, 0], centre1.R[plotting_lim, 1], s=1)
    for i in range(len(orbits)):
        if np.linalg.norm(orbits[i].R[plotting_lim, :]) <= 600:
            plt.scatter(orbits[i].R[plotting_lim, 0], orbits[i].R[plotting_lim, 1], s=0.4)
        else:
            unplotted.append(orbits[i].R[plotting_lim, :])
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    plt.tight_layout()
    plt.savefig('galaxyfinalXYZ.pdf', bbox="tight")
    print('number of unpolotted orbitals:', len(unplotted))

    unplotted2 = []
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.scatter(centre1.R[int(plotting_lim * 0.1), 0], centre1.R[int(plotting_lim * 0.1), 1], s=1)
    for i in range(len(orbits)):
        if np.linalg.norm(orbits[i].R[int(plotting_lim * 0.1), :]) <= 600:
            plt.scatter(orbits[i].R[int(plotting_lim * 0.1), 0], orbits[i].R[int(plotting_lim * 0.1), 1], s=0.2)
        else:
            unplotted2.append(orbits[i].R[int(plotting_lim * 0.1), :])
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    plt.tight_layout()
    plt.savefig('galaxy0.1XYZ.pdf', bbox="tight")
    print('number of unpolotted orbitals:', len(unplotted2))
