Category = {}
Category.None = 0
--Category.CommonTile = 1 << 1



--Category.Floor = 1 << 3
Category.NormalBox2DTile = 1 << 1

Category.Player = 1 << 2

Category.Type1SpaceShip = 1 << 3
Category.Type2SpaceShip = 1 << 4


--Category.Projectile = 1 << 5

Category.PlanetEarth = 1 << 6
Category.PlanetEnemy = 1 << 7

Category.UfoMissile = 1 << 8

Category.Laser_Projectile = 1 << 5

Category.Projectile = Category.Laser_Projectile | Category.UfoMissile

Category.PlayerProperties = Category.Player | Category.PlanetEarth
Category.Enemy = Category.Type1SpaceShip | Category.Type2SpaceShip | Category.PlanetEnemy

Category.SolidEntity = Category.Player | Category.Type1SpaceShip | Category.Type2SpaceShip