using Microsoft.VisualStudio.TestTools.UnitTesting;
using BrokenArrowSkinLab.Models;
using System;
using System.Numerics;

namespace BrokenArrowSkinLab.Tests;

[TestClass]
public class LayerTests
{
    [TestMethod]
    public void Layer_Creation_SetsDefaultValues()
    {
        // Arrange & Act
        var layer = new Layer();

        // Assert
        Assert.IsNotNull(layer.Id);
        Assert.AreEqual("New Layer", layer.Name);
        Assert.IsTrue(layer.IsVisible);
        Assert.AreEqual(1.0, layer.Opacity);
        Assert.AreEqual(Vector2.Zero, layer.Position);
        Assert.AreEqual(Vector2.One, layer.Scale);
        Assert.AreEqual(0f, layer.Rotation);
    }

    [TestMethod]
    public void Layer_Clone_CreatesNewInstance()
    {
        // Arrange
        var original = new Layer
        {
            Name = "Original Layer",
            Position = new Vector2(100, 200),
            Scale = new Vector2(2, 2),
            Rotation = 45f,
            Opacity = 0.5,
            IsVisible = false
        };

        // Act
        var clone = original.Clone();

        // Assert
        Assert.AreNotEqual(original.Id, clone.Id);
        Assert.AreEqual("Original Layer (Copy)", clone.Name);
        Assert.AreEqual(original.Position, clone.Position);
        Assert.AreEqual(original.Scale, clone.Scale);
        Assert.AreEqual(original.Rotation, clone.Rotation);
        Assert.AreEqual(original.Opacity, clone.Opacity);
        Assert.AreEqual(original.IsVisible, clone.IsVisible);
    }

    [TestMethod]
    public void Layer_Clone_WithGradient_ClonesGradient()
    {
        // Arrange
        var original = new Layer
        {
            Gradient = new GradientInfo
            {
                Type = GradientType.Radial,
                Angle = 90f
            }
        };

        // Act
        var clone = original.Clone();

        // Assert
        Assert.IsNotNull(clone.Gradient);
        Assert.AreNotSame(original.Gradient, clone.Gradient);
        Assert.AreEqual(GradientType.Radial, clone.Gradient.Type);
        Assert.AreEqual(90f, clone.Gradient.Angle);
    }

    [TestMethod]
    public void Layer_SetProperties_UpdatesCorrectly()
    {
        // Arrange
        var layer = new Layer();

        // Act
        layer.Name = "Test Layer";
        layer.Position = new Vector2(50, 75);
        layer.Opacity = 0.7;

        // Assert
        Assert.AreEqual("Test Layer", layer.Name);
        Assert.AreEqual(new Vector2(50, 75), layer.Position);
        Assert.AreEqual(0.7, layer.Opacity);
    }
}
