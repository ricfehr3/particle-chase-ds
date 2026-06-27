# Particle "Sim" For the Nintendo DS

A port of an older particle "Sim" I made to try out programming for the Nintendo DS. I was pleasantly surprised to see that [libnds](https://github.com/devkitPro/libnds) uses a simple opengl-_like_ interface. This is not optimized and there are no plans to work on it further, but I thought the result was fun enough to share.

<p align="center">
  <a href="https://rf3-games.itch.io/particle-chase-ds">
    <img
      src="https://img.shields.io/badge/PLAY%20%2F%20DOWNLOAD-Particle%20Chase%20DS-%23FF0B34?style=for-the-badge&logo=itch.io&logoColor=white&labelColor=222222"
      alt="Play / Download Particle Chase DS on itch.io"
      height="60"
    />
  </a>
</p>

![Particle Chase](https://github.com/user-attachments/assets/ac32a28f-e447-4cbe-afc0-2920c4f8b8e6)

# How to play

> [!WARNING]
> This only works on MelonDS on PC. Apologies, I have no idea why.

The "simulation" takes place in a boxed playfield where **Gravity Wells** act as forces on moving "**Entities**".

Use the **Stylus** to move **Gravity Wells** around the playfield.

# Summary of the Bottom Screen Values

| Name           | Simple Description                              | Control                          |
|----------------|------------------------------------------|----------------------------------|
| **Entities**   | Number of dots on screen.                | **Up** or **Down**  |
| **Grav Well**  | Strength of the gravity well.            | **Left** or **Right** |
| **Grav Floor** | Strength of the downwards.               | _Hold_ **R** and _press_ |
| **Mass Range** | Amount of weight entities can differ by. | _Hold_ **R** and _press_ **Left** or **Right** |
| **Drag**       | Percentage of entity velocity lost fixed scale. |  _Hold_ **L** and _press_ **Up** or **Down** |
| **Reset Vel**  | Max random velocity on _Reset_. | _Hold_ **L** and _press_ **Left** or **Right** |
| **Sim Time**  | Simulation speed. | _Hold_ **L** and _press_ **A** or **B** |
| **Direction**  | "Direction" of Gravity Well. | _Press_ **X** for **Attract** or **Y** for **Repulse** |
| **Mode**  | "Mode" of Gravity Well. **Normal**: The force is stronger ***closer*** to the Gravity Well. **Spring**: The force is stronger **Farther** from the Gravity Well. | _Press_ **A** for **Normal** Mode or **B** for **Spring** Mode |
| **Save Pos** | Save the Gravity Well at the stylus touch point. | _Touch_ the touchpad and _press_ **L** or **R** | 
| **Delete Last** | Delete the last placed Gravity Well. | _Hold_ **L** and _press_ **X** |
| **Reset Entities** | Randomize the position and velocity of Entities. | _Press_ **Start** |
| **Randomize Vels** | Randomize the velocity of Entities. | _Press_ **Select** |
| **Cycle Color** | Set Entity Color Mode to **Static**, **Speed**, or **Random**. | _Hold_ **R** and _press_ **Select** |
| **Reset All** | Reset game to initial state. | _Hold_ **R** and _press_ **Start** |

