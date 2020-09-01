module.exports = (sequelize, DataTypes) => {
  const Hosts = sequelize.define(
    "hosts",
    {
      HostID: {
        type: DataTypes.INTEGER,
        allowNull: false,
        primaryKey: true,
        autoIncrement: true,
      },
      HostName: {
        type: DataTypes.STRING,
        allowNull: false,
      },
      HostIP: {
        type: DataTypes.STRING,
        allowNull: false,
      },
    },
    { timestamps: false }
  );
  //   Users.associate = function (models) {
  //     Users.hasMany(models.Roles, {
  //       foreignKey: "role_id",
  //     });
  //   };
  return Hosts;
};
