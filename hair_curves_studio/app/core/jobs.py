"""Job system for background task execution"""

from typing import Callable, Any, Optional, Dict
from PySide6.QtCore import QObject, QThreadPool, QRunnable, Signal, Slot
import traceback
from .logger import logger


class JobSignals(QObject):
    """Signals for job status"""
    started = Signal(str)  # job_id
    progress = Signal(str, int)  # job_id, progress (0-100)
    finished = Signal(str, object)  # job_id, result
    error = Signal(str, str)  # job_id, error message
    cancelled = Signal(str)  # job_id


class Job(QRunnable):
    """Runnable job for thread pool"""
    
    def __init__(self, job_id: str, func: Callable, *args, **kwargs):
        super().__init__()
        self.job_id = job_id
        self.func = func
        self.args = args
        self.kwargs = kwargs
        self.signals = JobSignals()
        self._cancelled = False
        self.setAutoDelete(True)
    
    def cancel(self):
        """Cancel the job"""
        self._cancelled = True
    
    def is_cancelled(self) -> bool:
        """Check if job is cancelled"""
        return self._cancelled
    
    @Slot()
    def run(self):
        """Execute the job"""
        try:
            logger.debug(f"Job started: {self.job_id}")
            self.signals.started.emit(self.job_id)
            
            # Execute function
            result = self.func(*self.args, **self.kwargs)
            
            if self._cancelled:
                logger.debug(f"Job cancelled: {self.job_id}")
                self.signals.cancelled.emit(self.job_id)
            else:
                logger.debug(f"Job finished: {self.job_id}")
                self.signals.finished.emit(self.job_id, result)
        
        except Exception as e:
            error_msg = f"{str(e)}\n{traceback.format_exc()}"
            logger.error(f"Job error ({self.job_id}): {error_msg}")
            self.signals.error.emit(self.job_id, error_msg)


class JobManager:
    """Manages background jobs using QThreadPool"""
    
    _instance = None
    
    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance._initialized = False
        return cls._instance
    
    def __init__(self):
        if self._initialized:
            return
        
        self._initialized = True
        self.thread_pool = QThreadPool.globalInstance()
        self.thread_pool.setMaxThreadCount(4)
        
        self.jobs: Dict[str, Job] = {}
        self._job_counter = 0
        
        logger.info(f"JobManager initialized with {self.thread_pool.maxThreadCount()} threads")
    
    def submit(self,
               func: Callable,
               *args,
               job_id: Optional[str] = None,
               on_finished: Optional[Callable] = None,
               on_error: Optional[Callable] = None,
               on_progress: Optional[Callable] = None,
               **kwargs) -> str:
        """
        Submit a job to the thread pool
        
        Args:
            func: Function to execute
            *args: Positional arguments for func
            job_id: Optional job identifier (auto-generated if not provided)
            on_finished: Callback for job completion (receives result)
            on_error: Callback for job error (receives error message)
            on_progress: Callback for progress updates (receives progress 0-100)
            **kwargs: Keyword arguments for func
        
        Returns:
            job_id
        """
        if job_id is None:
            self._job_counter += 1
            job_id = f"job_{self._job_counter}"
        
        job = Job(job_id, func, *args, **kwargs)
        
        # Connect callbacks
        if on_finished:
            job.signals.finished.connect(lambda jid, result: on_finished(result))
        if on_error:
            job.signals.error.connect(lambda jid, error: on_error(error))
        if on_progress:
            job.signals.progress.connect(lambda jid, prog: on_progress(prog))
        
        # Clean up on completion
        job.signals.finished.connect(lambda jid: self._cleanup_job(jid))
        job.signals.error.connect(lambda jid, err: self._cleanup_job(jid))
        job.signals.cancelled.connect(lambda jid: self._cleanup_job(jid))
        
        self.jobs[job_id] = job
        self.thread_pool.start(job)
        
        logger.debug(f"Job submitted: {job_id}")
        return job_id
    
    def cancel(self, job_id: str) -> bool:
        """Cancel a job"""
        if job_id in self.jobs:
            self.jobs[job_id].cancel()
            logger.debug(f"Job cancellation requested: {job_id}")
            return True
        return False
    
    def _cleanup_job(self, job_id: str):
        """Remove job from tracking"""
        if job_id in self.jobs:
            del self.jobs[job_id]
            logger.debug(f"Job cleaned up: {job_id}")
    
    def wait_for_all(self):
        """Wait for all jobs to complete"""
        self.thread_pool.waitForDone()
    
    def active_job_count(self) -> int:
        """Get number of active jobs"""
        return self.thread_pool.activeThreadCount()
    
    def is_busy(self) -> bool:
        """Check if any jobs are running"""
        return self.active_job_count() > 0


# Global job manager instance
job_manager = JobManager()
