"""
Hello World example for dartpy2 (DART 7)
This demonstrates the basic usage of dartpy2 with snake_case API
"""

import dartpy2 as dart


def main():
    # Create a world
    world = dart.simulation.World()

    # Load robot using DartLoader
    urdf_parser = dart.io.DartLoader()
    kr5 = urdf_parser.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")
    ground = urdf_parser.parse_skeleton("dart://sample/urdf/KR5/ground.urdf")

    # Add skeletons to the world
    world.add_skeleton(kr5)
    world.add_skeleton(ground)
    print(f"Robot {kr5.get_name()} is loaded")

    # Run simulation for 100 steps
    for i in range(100):
        if i % 10 == 0:
            print(f"[{world.get_sim_frames()}] joint position: {kr5.get_positions()}")
        world.step()


if __name__ == "__main__":
    main()
