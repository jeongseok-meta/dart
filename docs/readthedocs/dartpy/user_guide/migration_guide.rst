===============
Migration Guide
===============

**From dartpy (DART 6.15) to dartpy2 (DART 7)**

Quick Reference
===============

This guide helps you migrate code from **dartpy** (DART 6.15) to **dartpy2** (DART 7).

Module Names
============

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - dartpy (DART 6.15)
     - dartpy2 (DART 7)
   * - ``import dartpy``
     - ``import dartpy2``
   * - ``from dartpy.utils import DartLoader``
     - ``from dartpy2.io import DartLoader``

Key Changes
===========

1. **Pythonic Naming**: All methods now use ``snake_case`` instead of ``camelCase``
2. **Module Reorganization**: ``utils`` → ``io`` for file I/O operations
3. **Factory Methods**: Some classes use explicit factory methods instead of constructors

Common Patterns
===============

Loading URDF Files
------------------

**dartpy (DART 6.15)**

.. code-block:: python

   from dartpy.utils import DartLoader

   loader = DartLoader()
   skeleton = loader.parseSkeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

**dartpy2 (DART 7)**

.. code-block:: python

   from dartpy2.io import DartLoader

   loader = DartLoader()
   skeleton = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

Creating Skeletons
------------------

**dartpy (DART 6.15)**

.. code-block:: python

   from dartpy.dynamics import Skeleton

   # Direct constructor
   skeleton = Skeleton("my_skeleton")

**dartpy2 (DART 7)**

.. code-block:: python

   from dartpy2.dynamics import Skeleton

   # Factory method (matches C++ API)
   skeleton = Skeleton.create("my_skeleton")
   # or
   skeleton = Skeleton.create()  # Empty skeleton

Working with Worlds
-------------------

**dartpy (DART 6.15)**

.. code-block:: python

   from dartpy.simulation import World

   world = World()
   world.addSkeleton(skeleton)
   world.setGravity([0, 0, -9.81])
   world.step()

**dartpy2 (DART 7)**

.. code-block:: python

   from dartpy2.simulation import World

   world = World()
   world.add_skeleton(skeleton)
   world.set_gravity([0, 0, -9.81])
   world.step()

Manipulating DOFs
-----------------

**dartpy (DART 6.15)**

.. code-block:: python

   # By index
   dof = skeleton.getDof(0)
   dof.setPosition(0.5)
   pos = dof.getPosition()

   # By name
   dof = skeleton.getDof("l_leg_hpy")
   dof.setPosition(0.3)

**dartpy2 (DART 7)**

.. code-block:: python

   # By index
   dof = skeleton.get_dof(0)
   dof.set_position(0.5)
   pos = dof.get_position()

   # By name
   dof = skeleton.get_dof("l_leg_hpy")
   dof.set_position(0.3)

Complete Example
================

**dartpy (DART 6.15)**

.. code-block:: python

   import dartpy
   from dartpy.utils import DartLoader
   from dartpy.simulation import World

   # Load robot
   loader = DartLoader()
   robot = loader.parseSkeleton("robot.urdf")

   # Create world
   world = World()
   world.addSkeleton(robot)
   world.setTimeStep(0.001)
   world.setGravity([0, 0, -9.81])

   # Set joint positions
   dof = robot.getDof("joint_name")
   dof.setPosition(0.5)

   # Simulate
   for i in range(1000):
       world.step()
       print(f"Time: {world.getTime()}")

**dartpy2 (DART 7)**

.. code-block:: python

   import dartpy2
   from dartpy2.io import DartLoader
   from dartpy2.simulation import World

   # Load robot
   loader = DartLoader()
   robot = loader.parse_skeleton("robot.urdf")

   # Create world
   world = World()
   world.add_skeleton(robot)
   world.set_time_step(0.001)
   world.set_gravity([0, 0, -9.81])

   # Set joint positions
   dof = robot.get_dof("joint_name")
   dof.set_position(0.5)

   # Simulate
   for i in range(1000):
       world.step()
       print(f"Time: {world.get_time()}")

API Reference Mapping
=====================

Skeleton Methods
----------------

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - dartpy (DART 6.15)
     - dartpy2 (DART 7)
   * - ``Skeleton(name)``
     - ``Skeleton.create(name)``
   * - ``skeleton.getName()``
     - ``skeleton.get_name()``
   * - ``skeleton.setName(name)``
     - ``skeleton.set_name(name)``
   * - ``skeleton.getNumDofs()``
     - ``skeleton.get_num_dofs()``
   * - ``skeleton.getNumBodyNodes()``
     - ``skeleton.get_num_body_nodes()``
   * - ``skeleton.getDof(index)``
     - ``skeleton.get_dof(index)``
   * - ``skeleton.getDof(name)``
     - ``skeleton.get_dof(name)``

DegreeOfFreedom Methods
------------------------

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - dartpy (DART 6.15)
     - dartpy2 (DART 7)
   * - ``dof.getName()``
     - ``dof.get_name()``
   * - ``dof.getPosition()``
     - ``dof.get_position()``
   * - ``dof.setPosition(pos)``
     - ``dof.set_position(pos)``
   * - ``dof.getVelocity()``
     - ``dof.get_velocity()``
   * - ``dof.setVelocity(vel)``
     - ``dof.set_velocity(vel)``
   * - ``dof.getForce()``
     - ``dof.get_force()``
   * - ``dof.setForce(force)``
     - ``dof.set_force(force)``
   * - ``dof.getPositionLimits()``
     - ``dof.get_position_limits()``
   * - ``dof.setPositionLimits(lower, upper)``
     - ``dof.set_position_limits(lower, upper)``
   * - ``dof.getIndexInSkeleton()``
     - ``dof.get_index_in_skeleton()``
   * - ``dof.getIndexInJoint()``
     - ``dof.get_index_in_joint()``

World Methods
-------------

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - dartpy (DART 6.15)
     - dartpy2 (DART 7)
   * - ``World()``
     - ``World()``
   * - ``world.addSkeleton(skel)``
     - ``world.add_skeleton(skel)``
   * - ``world.getNumSkeletons()``
     - ``world.get_num_skeletons()``
   * - ``world.setGravity(gravity)``
     - ``world.set_gravity(gravity)``
   * - ``world.getGravity()``
     - ``world.get_gravity()``
   * - ``world.setTimeStep(dt)``
     - ``world.set_time_step(dt)``
   * - ``world.getTimeStep()``
     - ``world.get_time_step()``
   * - ``world.getTime()``
     - ``world.get_time()``
   * - ``world.setTime(t)``
     - ``world.set_time(t)``
   * - ``world.getSimFrames()``
     - ``world.get_sim_frames()``

DartLoader Methods
------------------

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - dartpy (DART 6.15)
     - dartpy2 (DART 7)
   * - ``from dartpy.utils import DartLoader``
     - ``from dartpy2.io import DartLoader``
   * - ``loader.parseSkeleton(uri)``
     - ``loader.parse_skeleton(uri)``

Migration Tips
==============

Automated Migration
-------------------

For simple renaming, you can use search and replace:

1. **Module imports**: ``import dartpy`` → ``import dartpy2``
2. **Module paths**: ``dartpy.utils`` → ``dartpy2.io``
3. **Method naming**: Use regex to convert ``camelCase`` to ``snake_case``

Manual Updates Required
-----------------------

- **Skeleton creation**: Change from constructor to factory method
- **Complex method signatures**: Review API documentation for any parameter changes
- **Return types**: Some methods may return different types (e.g., tuples vs. custom classes)

Testing Your Migration
----------------------

After migration, verify:

1. All imports resolve correctly
2. Factory methods are used where required
3. Method names follow ``snake_case`` convention
4. Test your simulation to ensure behavior is unchanged

Benefits of dartpy2
===================

- **Better Performance**: Built with nanobind for faster Python-C++ interop
- **Smaller Binary Size**: More efficient memory usage
- **Modern Python**: Better type hints and IDE support
- **Consistency**: Follows Python naming conventions (PEP 8)
- **Maintainability**: Cleaner codebase aligned with DART 7

Getting Help
============

- **Documentation**: https://dartsim.github.io/
- **GitHub Issues**: https://github.com/dartsim/dart/issues
- **Community**: Join our discussions on GitHub

For detailed API documentation, see the :doc:`API Reference <../api/index>`.
