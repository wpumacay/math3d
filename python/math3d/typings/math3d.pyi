from typing import Union
from numpy import ndarray

################################################################################
#                           Type hints for Vector2                             #
################################################################################

class Vector2:
    def __init__(
        self, *args: Union[float, ndarray], **kwargs: Union[float, ndarray]
    ): ...
    @property
    def x(self) -> float: ...
    @x.setter
    def x(self, value: float) -> None: ...
    @property
    def y(self) -> float: ...
    @y.setter
    def y(self, value: float) -> None: ...
    def __add__(self, rhs: Vector2) -> Vector2: ...
    def __sub__(self, rhs: Vector2) -> Vector2: ...
    def __mul__(self, scale: float) -> Vector2: ...
    def __rmul__(self, scale: float) -> Vector2: ...
    def __eq__(self, other: Vector2) -> bool: ...
    def __getitem__(self, index: int) -> float: ...
    def __setitem__(self, index: int, value: float) -> None: ...
    def __neq__(self, other: Vector2) -> bool: ...
    def __neg__(self) -> Vector2: ...
    def dot(self, rhs: Vector2) -> float: ...
    def norm(self) -> float: ...
    def squareNorm(self) -> float: ...
    def normalize(self) -> Vector2: ...
    def normalize_(self) -> None: ...

# Structure should be the same, except for the internal usage of f32 and f64
Vector2f = Vector2
Vector2d = Vector2

################################################################################
#                           Type hints for Vector3                             #
################################################################################

class Vector3:
    def __init__(
        self, *args: Union[float, ndarray], **kwargs: Union[float, ndarray]
    ): ...
    @property
    def x(self) -> float: ...
    @x.setter
    def x(self, value: float) -> None: ...
    @property
    def y(self) -> float: ...
    @y.setter
    def y(self, value: float) -> None: ...
    @property
    def z(self) -> float: ...
    @z.setter
    def z(self, value: float) -> None: ...
    def __add__(self, rhs: Vector3) -> Vector3: ...
    def __sub__(self, rhs: Vector3) -> Vector3: ...
    def __mul__(self, scale: float) -> Vector3: ...
    def __rmul__(self, scale: float) -> Vector3: ...
    def __eq__(self, other: Vector3) -> bool: ...
    def __getitem__(self, index: int) -> float: ...
    def __setitem__(self, index: int, value: float) -> None: ...
    def __neq__(self, other: Vector3) -> bool: ...
    def __neg__(self) -> Vector3: ...
    def cross(self, rhs: Vector3) -> Vector3: ...
    def dot(self, rhs: Vector3) -> float: ...
    def norm(self) -> float: ...
    def squareNorm(self) -> float: ...
    def normalize(self) -> Vector3: ...
    def normalize_(self) -> None: ...

# Structure should be the same, except for the internal usage of f32 and f64
Vector3f = Vector3
Vector3d = Vector3

################################################################################
#                           Type hints for Vector4                             #
################################################################################

class Vector4:
    def __init__(
        self, *args: Union[float, ndarray], **kwargs: Union[float, ndarray]
    ): ...
    @property
    def x(self) -> float: ...
    @x.setter
    def x(self, value: float) -> None: ...
    @property
    def y(self) -> float: ...
    @y.setter
    def y(self, value: float) -> None: ...
    @property
    def z(self) -> float: ...
    @z.setter
    def z(self, value: float) -> None: ...
    @property
    def w(self) -> float: ...
    @w.setter
    def w(self, value: float) -> None: ...
    def __add__(self, rhs: Vector4) -> Vector4: ...
    def __sub__(self, rhs: Vector4) -> Vector4: ...
    def __mul__(self, scale: float) -> Vector4: ...
    def __rmul__(self, scale: float) -> Vector4: ...
    def __eq__(self, other: Vector4) -> bool: ...
    def __getitem__(self, index: int) -> float: ...
    def __setitem__(self, index: int, value: float) -> None: ...
    def __neq__(self, other: Vector4) -> bool: ...
    def __neg__(self) -> Vector4: ...
    def dot(self, rhs: Vector4) -> float: ...
    def norm(self) -> float: ...
    def squareNorm(self) -> float: ...
    def normalize(self) -> Vector4: ...
    def normalize_(self) -> None: ...

# Structure should be the same, except for the internal usage of f32 and f64
Vector4f = Vector4
Vector4d = Vector4

################################################################################
#                          Type hints for Quaternion                           #
################################################################################

class Quaternion:
    def __init__(
        self, *args: Union[float, ndarray], **kwargs: Union[float, ndarray]
    ): ...
    @property
    def w(self) -> float: ...
    @w.setter
    def w(self, value: float) -> None: ...
    @property
    def x(self) -> float: ...
    @x.setter
    def x(self, value: float) -> None: ...
    @property
    def y(self) -> float: ...
    @y.setter
    def y(self, value: float) -> None: ...
    @property
    def z(self) -> float: ...
    @z.setter
    def z(self, value: float) -> None: ...
    def __add__(self, rhs: Quaternion) -> Quaternion: ...
    def __sub__(self, rhs: Quaternion) -> Quaternion: ...
    def __mul__(self, rhs: Union[float, Quaternion]) -> Quaternion: ...
    def __rmul__(self, rhs: Union[float, Quaternion]) -> Quaternion: ...
    def __getitem__(self, index: int) -> float: ...
    def __setitem__(self, index: int, value: float) -> None: ...
    def __eq__(self, other: Quaternion) -> bool: ...
    def __neq__(self, other: Quaternion) -> bool: ...
    def length(self) -> float: ...
    def normalize(self) -> Quaternion: ...
    def normalize_(self) -> None: ...
    def conjugate(self) -> Quaternion: ...
    def inverse(self) -> Quaternion: ...
    def rotate(self, vec: Union[Vector3, ndarray]) -> Vector3: ...
    @staticmethod
    def RotationX(angle: float) -> Quaternion: ...
    @staticmethod
    def RotationY(angle: float) -> Quaternion: ...
    @staticmethod
    def RotationZ(angle: float) -> Quaternion: ...
