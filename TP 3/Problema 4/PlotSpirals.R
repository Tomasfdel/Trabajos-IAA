puntos <- read.table("./Resultados spirals/16.predic",
                     sep = "\t",
                     header = FALSE)

plot(x = 1,
     type = "n",
     xlim = c(-1.25, 1.25), 
     ylim = c(-1.25, 1.25),
     xlab = "", 
     ylab = "",
     main = "Espirales Anidadas, N_Bins = 16")

segments(-1.5, 0, 1.5, 0)
segments(0, -1.5, 0, 1.5)
     
points(x = puntos$V1[puntos$V3 >= 0.5],
       y = puntos$V2[puntos$V3 >= 0.5],
       pch=16,cex=0.6,
       col = "skyblue3")
points(x = puntos$V1[puntos$V3 < 0.5],
       y = puntos$V2[puntos$V3 < 0.5],
       pch=16,cex=0.6,
       col = "tomato3")

