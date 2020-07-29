puntos <- read.table("./3000spirals.prediction",
                     sep = "\t",
                     header = FALSE)

plot(x = 1,
     type = "n",
     xlim = c(-1.25, 1.25), 
     ylim = c(-1.25, 1.25),
     xlab = "", 
     ylab = "",
     main = "Predicción 3000 puntos")

segments(-1.5, 0, 1.5, 0)
segments(0, -1.5, 0, 1.5)
     
points(x = puntos$V1[puntos$V3 == 1],
       y = puntos$V2[puntos$V3 == 1],
       pch=16,cex=0.4,
       col = "skyblue3")
points(x = puntos$V1[puntos$V3 == 0],
       y = puntos$V2[puntos$V3 == 0],
       pch=16,cex=0.4,
       col = "tomato3")

