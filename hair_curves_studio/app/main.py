"""Main application entry point"""

import sys
from pathlib import Path
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import Qt

from .ui.main_window import MainWindow
from .core.logger import logger


def main():
    """Application entry point"""
    # Enable high DPI scaling
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)
    
    # Create application
    app = QApplication(sys.argv)
    app.setApplicationName("Broken Arrow Hair Curves Studio")
    app.setOrganizationName("Broken Arrow Software")
    
    # Apply dark stylesheet
    app.setStyle("Fusion")
    from PySide6.QtGui import QPalette, QColor
    
    palette = QPalette()
    palette.setColor(QPalette.Window, QColor(53, 53, 53))
    palette.setColor(QPalette.WindowText, Qt.white)
    palette.setColor(QPalette.Base, QColor(35, 35, 35))
    palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
    palette.setColor(QPalette.ToolTipBase, QColor(25, 25, 25))
    palette.setColor(QPalette.ToolTipText, Qt.white)
    palette.setColor(QPalette.Text, Qt.white)
    palette.setColor(QPalette.Button, QColor(53, 53, 53))
    palette.setColor(QPalette.ButtonText, Qt.white)
    palette.setColor(QPalette.BrightText, Qt.red)
    palette.setColor(QPalette.Link, QColor(42, 130, 218))
    palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
    palette.setColor(QPalette.HighlightedText, Qt.black)
    
    app.setPalette(palette)
    
    # Enable logging to file
    log_dir = Path.home() / ".hair_curves_studio" / "logs"
    logger.enable_file_logging(log_dir)
    
    logger.info("Application starting...")
    
    # Create and show main window
    window = MainWindow()
    window.show()
    
    # Run application
    return_code = app.exec()
    
    logger.info("Application exiting...")
    return return_code


if __name__ == "__main__":
    sys.exit(main())
