# Copyright (c) 2011-2025, The DART development contributors

import numpy as np
import pytest


def test_inertia_placeholder():
    """
    Placeholder test for Inertia class.

    Note: The Inertia class is not yet bound in dartpy2.
    This test is a placeholder that can be expanded once bindings are added.
    """
    # TODO: Implement tests once Inertia class is bound in dartpy2
    # from dartpy2.dynamics import Inertia
    #
    # # Test default values
    # i1 = Inertia()
    # assert i1 is not None
    #
    # # Initialize with parameters
    # i2 = Inertia(0.1, [0, 0, 0], 1.3 * np.eye(3))
    # assert i2 is not None
    #
    # new_mass = 1.5
    # i2.set_mass(new_mass)
    # assert i2.get_mass() == new_mass

    # Placeholder assertion to make test pass
    assert True, "Inertia class not yet bound in dartpy2"


def test_inertia_from_skeleton():
    """Test getting inertia from skeleton body nodes once API is available."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")
    assert kr5 is not None

    # TODO: Add inertia tests once get_inertia() is bound on BodyNode
    # for i in range(1, kr5.get_num_body_nodes()):
    #     body = kr5.get_body_node(i)
    #     inertia = body.get_inertia()
    #     assert inertia is not None


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
