puntos <- read.table("./xor.data",
                     sep = ",",
                     header = FALSE)

plot(x = 1,
     type = "n",
     xlim = c(-1.25, 1.25), 
     ylim = c(-1.25, 1.25),
     xlab = "", 
     ylab = "",
     main = "XOR Problem")

segments(-1.5, 0, 1.5, 0)
segments(0, -1.5, 0, 1.5)
     
points(x = puntos$V1[puntos$V7 == 1],
       y = puntos$V2[puntos$V7 == 1],
       pch=16,cex=1,
       col = "skyblue3")
points(x = puntos$V1[puntos$V7 == 0],
       y = puntos$V2[puntos$V7 == 0],
       pch=16,cex=1,
       col = "tomato3")

